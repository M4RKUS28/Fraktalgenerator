#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , currentImg(0, START_SCALE, START_POS_X , START_POS_Y ), lastImgStructID(0),
      ignoreXPosEdited(false),
      ignoreYPosEdited(false),
      ui(new Ui::MainWindow)
{
    //setup ui
    ui->setupUi(this);
    ui->comboBox_palette->setCurrentIndex(0);
    ui->stackedWidgetExtraSettings->setHidden(true);

    //save start size
    int width = (int)(QApplication::screens().at(0)->availableGeometry().width() *this->devicePixelRatio()* 0.075) * 10,
        height = (int)(QApplication::screens().at(0)->availableGeometry().height() *this->devicePixelRatio()* 0.085) * 10;

    //setup currentImage
    currentImg.init(width, height, ui->spinBoxMaxIterations->value(), true);
    currentImg.painter->fillRect(currentImg.image->rect(), QColor(ui->comboBox_background_color->currentText()));

    // connect signals
    connect(ui->imageView, SIGNAL(mouseMove(QPoint)), this, SLOT(mouse_move_in_img(QPoint)));

    // init rect:
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    zoomRect.updateRectPos(Point(currentImg.mapGausReToImgPos(currentImg.gaus_mid_re), currentImg.mapGausImToImgPos(currentImg.gaus_mid_im)));

    //setup ui values
    this->ui->spinBoxW->setValue( width );
    this->ui->spinBoxH->setValue( height );

    //start timer
    timerID = this->startTimer(20);

    updateImage();
    this->startRefresh(currentImg, true);

}

MainWindow::~MainWindow()
{
    //Stoppe timer
    this->killTimer(timerID);

    // Stoppe und lösche Threads...
    stopThreads();

    //Lösche die Bilderliste
    for( auto e : settingsList)
        e.cleanUP();
    settingsList.clear();

    delete ui;
    ui = nullptr;

}

void MainWindow::updateMidPos(bool clear)
{

    this->ignoreXPosEdited = true;
    this->ignoreYPosEdited = true;
    if(clear) {

        ui->re->setValue(0);
        ui->im->setValue(0);
    } else {
        ui->re->setValue(currentImg.mapImgPosReToGaus(this->zoomRect.getMousePos().x()));
        ui->im->setValue(currentImg.mapImgPosImToGaus(this->zoomRect.getMousePos().y()));
        ui->statusbar->showMessage("Re(" + QString::number(currentImg.mapImgPosReToGaus(this->zoomRect.getMousePos().x()))
                                   + ") Im(" + QString::number(currentImg.mapImgPosImToGaus(this->zoomRect.getMousePos().y())) + ")", 2000);

    }

}

void MainWindow::timerEvent(QTimerEvent *)
{
    zustandWechseln("TIMER");
}


void MainWindow::zustandWechseln(QString aktion, QString, QPoint m_pos, QMouseEvent *m_event)
{
    if(aktion != "TIMER" && aktion != "mouseMoveEvent")
        qDebug() << "zustandWechseln "<< aktion;

    switch (this->state) {
    case MainWindow::STOPED:

        if(aktion == "TIMER") {

        } else if(aktion == "BUTTON_REFRESH_AND_STOP") {
            qDebug() << this->op_mode;

            if(this->op_mode == OP_MODE::REFRESH ) {
                //Aktualiseren
                this->startRefresh(currentImg);

            // wenn: einstellungenverändert == true:
            } else if(this->op_mode == OP_MODE::APPLY_SETTINGS || this->op_mode == OP_MODE::ZOOM_TO_SELECTED || this->op_mode == OP_MODE::APPLY_SETTINGS_AND_ZOOM) {
                //Zoomen
                ImageSetting newSetting = getNewScaledSetting(currentImg);
                this->startRefresh(newSetting, true);

            }

        } else if( aktion == "DOPPELKLICK_IN_BILD" && m_event) {

            //update rect if no mousemove or mouse press was done --> fix doppelclick no zoom just refresh error
            zustandWechseln("mousePressEvent", "", QPoint(), m_event);

            auto pos = this->ui->imageView->mapFrom(this, m_event->pos() * ui->imageView->getHdpi_multiplicator());

            if(m_event->button() == Qt::LeftButton && pos.x() >= 0 && pos.x() < currentImg.img_w && pos.y() >= 0 && pos.y() < currentImg.img_h) {
                ImageSetting newSetting = getNewScaledSetting(currentImg);
                this->startRefresh(newSetting, true);

            }
        } else if(aktion == "mouseReleaseEvent" && m_event) {
            if(m_event->button() == Qt::LeftButton) {
                // no update in paintevent!! --> no move!!
                this->zoomRect.setMousePressState(false);
                this->update();
            }

        } else if(aktion == "mouseMoveEvent") {
            auto qpos = m_pos; // this->ui->imageView->mapFrom(this, m_pos);
            auto pos = Point(qpos.x(), qpos.y());
//            qDebug() << qpos;

            if(pos.x() >= 0 && pos.x() < currentImg.img_w && pos.y() >= 0 && pos.y() < currentImg.img_h) {
                this->zoomRect.updateRectPos( pos );

                if(this->zoomRect.rightMouseIsPressed()) {
                    this->setOperationMode(OP_MODE::ZOOM_TO_SELECTED);
                    this->updateImage();
                }
                ui->label_iterations->setText(QString::number(currentImg.getIterationCountAt(qpos)));

                updateMidPos();


                if( ui->radioButton_zahelnfolge_bei_mousemove->isChecked() && zahlenfolge.isShown() ) {
                    this->zahlenfolge.setZahlenfolge(pos, currentImg);
                    this->updateImage();
                }

            } else {
                ui->label_iterations->setText("-");
//                ui->re->setValue(0);
//                ui->im->setValue(0);
            }

        } else if(aktion == "mousePressEvent"  && m_event) {

            auto qpos = this->ui->imageView->mapFrom(this, m_event->pos()) * ui->imageView->getHdpi_multiplicator();
            auto pos = Point(qpos.x(), qpos.y());

            if(pos.x() >= 0 && pos.x() < currentImg.img_w && pos.y() >= 0 && pos.y() < currentImg.img_h
                    && m_event->pos().x() < ui->scrollArea->width() + ui->scrollArea->pos().x() + 10
                    && m_event->pos().y() < ui->scrollArea->height() + ui->scrollArea->pos().y() + 10) {
                if(m_event->button() == Qt::LeftButton) {
                    zoomRect.setMousePressState(true);
                    zoomRect.show();

                    this->zoomRect.updateRectPos(pos);
                    this->updateImage();

                    this->setOperationMode(OP_MODE::ZOOM_TO_SELECTED);

                    ui->label_iterations->setText(QString::number(currentImg.getIterationCountAt(qpos)));

                    updateMidPos();

                    if(zahlenfolge.isShown())
                        this->zahlenfolge.removeZahlenfolge();
                } else  if(m_event->button() == Qt::RightButton) {
                    this->zahlenfolge.setZahlenfolge(pos, currentImg);
                }

                this->updateImage();
            } else {
                if(m_event->button() != Qt::LeftButton) {
                    this->setOperationMode(OP_MODE::REFRESH);
                    zoomRect.setMousePressState(false);
                    zoomRect.hide();

    //                updateMidPos(true);
                }

                ui->label_iterations->setText("-");
                this->zahlenfolge.removeZahlenfolge();

                updateImage();
            }


        } else if(aktion == "ZURÜCK") {
            int pos = - 1;
            for(int i = 0; i < settingsList.length(); i++)
                if(settingsList.at(i).image == currentImg.image)
                    pos = i - 1;

            if(pos >= 0 && pos < settingsList.length()) {
                //HIER AKTUELLES AUWÄHLEN!!!!
                //ui->listWidgetHistory->setCurrentRow(pos);

                this->currentImg = settingsList.at(pos);
                if(ui->radioButton_reload_at_back->isChecked())
                    this->startRefresh(currentImg);
                else {
                    this->endRefresh(false);
                    updateImage();
                }

                for(int i = 0; i < ui->listWidgetHistory->count(); i++)
                    if(ui->listWidgetHistory->item(i)->data(187).toInt() == currentImg.id)
                        ui->listWidgetHistory->setCurrentRow(i);
                ui->pushButton_vor->setDisabled(false);
            }

            if(pos == 0)
                ui->pushButton_back->setEnabled(false);

        } else if(aktion == "VOR") {
            int pos = - 1;
            for(int i = 0; i < settingsList.length(); i++)
                if(settingsList.at(i).image == currentImg.image)
                    pos = i + 1;

            if(pos >= 0 && pos < settingsList.length()) {
                this->currentImg = settingsList.at(pos);
                if(ui->radioButton_reload_at_back->isChecked())
                    this->startRefresh(currentImg);
                else {
                    this->endRefresh(false);
                    updateImage();
                }

                for(int i = 0; i < ui->listWidgetHistory->count(); i++)
                    if(ui->listWidgetHistory->item(i)->data(187).toInt() == currentImg.id)
                        ui->listWidgetHistory->setCurrentRow(i);
                ui->pushButton_back->setDisabled(false);
            }

            ui->pushButton_vor->setDisabled(false);
            if(pos == settingsList.length()-1)
                ui->pushButton_vor->setDisabled(true);


        } else if(aktion == "HOME") {
            this->currentImg = settingsList.at(0);
            if(ui->radioButton_reload_at_back->isChecked())
                this->startRefresh(currentImg);
            else {
                this->endRefresh(false);
                updateImage();
            }

            for(int i = 0; i < ui->listWidgetHistory->count(); i++)
                if(ui->listWidgetHistory->item(i)->data(187).toInt() == currentImg.id)
                    ui->listWidgetHistory->setCurrentRow(i);
            ui->pushButton_back->setDisabled(true);
            ui->pushButton_vor->setDisabled(false);

        }
        break;
    case MainWindow::RUNNING:
        if(aktion == "TIMER") {
            if(imgUpdateNeeded)
                updateImage();
            if(checkForFinished())
                endRefresh(op_mode != OP_MODE::REFRESH);

        } else if(aktion == "BUTTON_REFRESH_AND_STOP") {
            this->stopThreads();
            this->endRefresh(op_mode != OP_MODE::REFRESH);
        }

        break;

    }
}

ImageSetting MainWindow::getNewScaledSetting(ImageSetting last_img)
{
    // Zoom midposize_t --> new scaling!!!!
    double midx =0 ;
    double midy =0 ;
    double nscale = 0;

    // 4 möglichkeiten: wechsel von mandelbrot zu julia --> calc julia an stelle mauspos der mandelbrot menge
    //  wechsel von julia zu mandelbrot --> starteinstellungen der mandelbrot menge
    // zoom in fraktal ( mit speicherung der alten julia einstellungen ) oder reload --> kein zoom

    // 1)
    if(last_img.isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 1 /*julia*/ ) {
        double c_x = 0.0, c_y = 0.0;
        if(this->zoomRect.isShown()) {

            // Anti Rundungsfehler!!
            if(ui->re->value() != 0.0 )
                c_x   = ui->re->value();
            else
                c_x   = last_img.mapImgPosReToGaus(this->zoomRect.getMousePos().x());

            if(ui->im->value() != 0.0 )
                c_y   = -ui->im->value();
            else
                c_y   = last_img.mapImgPosImToGaus(this->zoomRect.getMousePos().y());

        } else {
            c_x = last_img.gaus_mid_re;
            c_y = last_img.gaus_mid_im;
        }

        qDebug() << "create julia";

        //create new julia-funktion on midbpoint of last mandelbrot-funktion
        ImageSetting j( (this->lastImgStructID = this->lastImgStructID + 1), START_SCALE, START_POS_X_JUL, START_POS_Y_JUL );
        j.init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBoxMaxIterations->value(), false /*julia menge!*/ );
        j.juliaStart_real = c_x;
        j.juliaStart_img = c_y;

//        j.isStart = true; // save start for hom button
        return j;

        // 2)
    } else if( ! last_img.isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 0 /*mandelbrot*/) {
        ImageSetting m((this->lastImgStructID = this->lastImgStructID + 1), START_SCALE, START_POS_X_JUL, START_POS_Y_JUL );
        m.init(ui->spinBoxW->value(), ui->spinBoxH->value(), ui->spinBoxMaxIterations->value(), true);
        return m;

    } else {

        //es wird davon ausgegangen, dass diese Funktion dann aufgerufen wird, wenn mousePress gesetzt ist! Wenn nicht, dann sollte lastSetGeladen werden.
        // entweder zoomfeld gesetzt und davon mitte, egal ob doppelklick oder button, oder reines neu laden button!!
        double relativeZoom = ui->spinBox_zoom->value();
        if(ui->spinBox_zoom->value() == 0.0)
            ui->spinBox_zoom->setValue( (relativeZoom = 1.0) );

        //3)
       if(this->zoomRect.isShown() /*&& (last_img.scale * relativeZoom) < (std::pow(10, 16))*/) {
            if(relativeZoom > 0) {
                nscale = last_img.scale() * relativeZoom;
                midx   = last_img.mapImgPosReToGaus( this->zoomRect.getMousePos().x() );
                midy   = last_img.mapImgPosImToGaus(this->zoomRect.getMousePos().y() );
            } else {
                nscale  = -last_img.scale() / relativeZoom;
                midx    = last_img.mapImgPosReToGaus(this->zoomRect.getMousePos().x());
                midy    = last_img.mapImgPosImToGaus(this->zoomRect.getMousePos().y() );
            }

            //4)
        } else {
//           if((last_img.scale * relativeZoom) > (std::pow(10, 16)))
//               QMessageBox::warning(this, "Vergrößerungslimit Limit erreicht!", "Vergrößerungslimit Limit erreicht! Es wird die letzte Zoomstufe verwendet.");
            nscale = last_img.scale() ;
            midx = last_img.gaus_mid_re;
            midy = last_img.gaus_mid_im;
        }


        ImageSetting s((this->lastImgStructID = this->lastImgStructID + 1), nscale, midx, midy);
        s.init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBoxMaxIterations->value(), ui->comboBox_Fraktal->currentIndex() == 0 /*is mandelbrot*/ );

        if(ui->comboBox_Fraktal->currentIndex() == 1) {
            s.juliaStart_img = last_img.juliaStart_img;
            s.juliaStart_real = last_img.juliaStart_real;
        }
        return s;
    }

}

void MainWindow::updateUiWithImageSetting(ImageSetting imgs)
{
    // nach zurück oder home -> einstellungen laden
    this->ui->spinBoxW->setValue(imgs.img_w);
    this->ui->spinBoxH->setValue(imgs.img_h);
    this->ui->spinBoxMaxIterations->setValue(imgs.maxIterations);
    ui->comboBox_Fraktal->setCurrentIndex(imgs.isMandelbrotSet ? 0 : 1);
    ui->comboBox_palette->setCurrentIndex(imgs.palette);
    ui->radioButton_normalized->setChecked( imgs.normalized );
    ui->radioButton_invert->setChecked( imgs.inverted );

}

void MainWindow::startRefresh(ImageSetting set, bool appendToList)
{
    if(set.scale() == 0) {
        perror("INVALID SETTING!");
        return;
    }

    //reset ui
    this->state = STATE::RUNNING;
    ui->widget->setDisabled(true);
    ui->pushButtonStart->setText("Abbrechen");
    ui->progressBar->setEnabled(true);
    ui->frameButtons->setEnabled(false);

    // Reset Zahlenfolge
    zahlenfolge.removeZahlenfolge();

    //calc work for each thread
    size_t tc = std::max(std::min( ui->spinBox_threads->value(), (int)set.img_w), 1);
    ssize_t parts = set.img_w / tc;

    //fill image with background color:
    set.painter->fillRect(set.image->rect(), QColor(ui->comboBox_background_color->currentText()));
    this->updateImage();

    //reset progress bar
    this->ui->progressBar->setMaximum(set.img_w);
    this->ui->progressBar->setValue(0);

    //
    set.setColorSettings(this->ui->comboBox_palette->currentIndex(),
                         ui->radioButton_normalized->isChecked(),
                         log(log(ui->doubleSpinBoxEscapeR->value())),
                         ui->groupBoxMandelFarbe->isChecked(),
                         ui->comboBoxMandelColor->currentText(),
                         ui->radioButton_invert->isChecked(),
                         ui->doubleSpinBoxEscapeR->value());

    set.rgb1[0] = QColor::fromRgb(ui->spinBox_rgb1_r->value(), ui->spinBox_rgb1_g->value(), ui->spinBox_rgb1_b->value());
    set.rgb1[1] = QColor::fromRgb(ui->spinBox_rgb2_r->value(), ui->spinBox_rgb2_g->value(), ui->spinBox_rgb2_b->value());
    set.rgb1[2] = QColor::fromRgb(ui->spinBox_rgb3_r->value(), ui->spinBox_rgb3_g->value(), ui->spinBox_rgb3_b->value());

    set.hsv_saturation = ui->spinBoxHSV_satursion->value();
    set.spinBoxHSV_value = ui->spinBoxHSV_value->value();
    set.spinBoxHSV_alpha = ui->spinBoxHSV_alpha->value();

    set.farbwechselIntervall = ui->spinBoxFarbWechselIntervall->value();

    // add new settings to list ( bei neuladen wird setting nicht hinzugefügt )
    if(appendToList) {
        settingsList.push_back(set);
        currentImg = set;
    }

    //start workers
    for( size_t tn = 0; tn < tc; tn++) {
        WorkerThread * wt = new WorkerThread(this);
        this->tworkers.append( wt );

        const ssize_t x_left_corner_part = (parts * tn);
        const ssize_t x_right_corner_part = (parts * (tn + 1) + ( ( tn + 1 == tc ) ? (set.img_w - parts * (tn + 1)) : 0));
        const ssize_t y_top_corner = 0;
        const ssize_t y_bottom_corner = set.img_h;

        wt->setRange(x_left_corner_part, x_right_corner_part, y_top_corner, y_bottom_corner);
        wt->startCalc(set, this->ui->comboBox_precession->currentIndex());

        connect(wt, SIGNAL(finishedLine(QList<Pixel>*)), this, SLOT(finishedLine(QList<Pixel>*)));
        connect(wt, SIGNAL(finished()), this, SLOT(threadFinished()));

        // warte 20ms für den nächsten Thread
        QTime dieTime= QTime::currentTime().addMSecs(20);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}


#include <algorithm>


void MainWindow::afterColoring(ImageSetting set)
{
    size_t total = 0;
    QImage img;

    switch(set.palette) {
        case 6:
        if( ! set.hueIsSetted() ) {
            ui->progressBar->setMaximum( set.img_w);
            ui->progressBar->setValue(0);
            QApplication::processEvents();

            for (size_t i = 0; i <= set.maxIterations; i++)
                total += set.getNumIterationsPerPixelAt(i);
            double ** hue = new double*[set.img_w];
            for(int i = 0; i < set.img_w; i++) {
                hue[i] = new double[set.img_h];
                memset(hue[i], 0, sizeof(double) * set.img_h );
            }

            for(ssize_t x = 0; x < set.img_w; x++) {
                for(ssize_t y = 0; y < set.img_h; y++) {
                    size_t iteration = set.getIterationCountAt(x, y);
                    for (size_t i = 0; i <= iteration; i++)
                        hue[x][y] += (double)set.getNumIterationsPerPixelAt(i) / (double)total; /* Must be floating-posize_t division. */
                }
                ui->progressBar->setValue(ui->progressBar->value() + 1);
            }
            set.setHue(hue, total);
            ui->progressBar->setValue(ui->progressBar->maximum());
        }
        for(ssize_t x = 0; x < set.img_w; x++) {
            for(ssize_t y = 0; y < set.img_h; y++) {
                if(set.getIterationCountAt(x, y) == set.maxIterations) {
                    if(set.fixedColor)
                        currentImg.painter->setPen(QColor::fromHsv(set.hue[x][y] * 359, 255 , 255));
                    else
                        currentImg.painter->setPen(Qt::black);
                } else {
                    currentImg.painter->setPen(QColor::fromHsv(set.hue[x][y] * 359, 255 , 255));
                }
                currentImg.painter->drawPoint(x, y);

            }
        }
        ui->progressBar->setValue(ui->progressBar->maximum());
        break;
    }

    if( ! img.isNull()) {
        currentImg.image->swap(img);
        updateImage();
    }
}

void MainWindow::setOperationMode(OP_MODE o)
{
    //Refresh -->
    switch (this->op_mode) {

    case MainWindow::ZOOM_TO_SELECTED:
        // add change settings
        if(o == OP_MODE::APPLY_SETTINGS) {
            this->op_mode = OP_MODE::APPLY_SETTINGS_AND_ZOOM;
        } else {
            this->op_mode = o;
        }
        break;

    case MainWindow::REFRESH:
        if(o == OP_MODE::REFRESH) {

        } else {
            this->op_mode = o;
        }
        break;

    case MainWindow::APPLY_SETTINGS:
        // add zoom
        if(o == OP_MODE::ZOOM_TO_SELECTED) {
            this->op_mode = OP_MODE::APPLY_SETTINGS_AND_ZOOM;

        // ignore set refresh
        } else if(o == OP_MODE::REFRESH) {

        } else {
            this->op_mode = o;
        }
        break;

    case MainWindow::APPLY_SETTINGS_AND_ZOOM:
        // JUST REMOVE ZOOM
        if(o == OP_MODE::REFRESH) {
            this->op_mode = OP_MODE::APPLY_SETTINGS;

        //ignore set to one if both are settet
        } else if((o == OP_MODE::ZOOM_TO_SELECTED || o == OP_MODE::APPLY_SETTINGS)) {

        } else {
            this->op_mode = o;
        }
        break;

    case MainWindow::CALC_JULIA:
        if(o == OP_MODE::BACK_TO_MANDELBROT) {
            this->op_mode = OP_MODE::REFRESH;

            // ignore other
        } else {
        }
        break;

//    case MainWindow::CALC_JULIA_SETTINGS:
//        if(o == OP_MODE::BACK_TO_MANDELBROT) {
//            this->op_mode = OP_MODE::APPLY_SETTINGS;

//            // ignore other
//        } else {
//        }
//        break;

//    case MainWindow:::
//        if(o == OP_MODE::REFRESH) {

//        } else {
//            this->op_mode = o;
//        }
//        break;
    case MainWindow::RESET:
        if(o == OP_MODE::REFRESH) {

        } else {
            this->op_mode = o;
        }
        break;

    default:
        break;
    }

    this->setOperationBasedTexts();
}

void MainWindow::setOperationBasedTexts()
{
    switch (this->op_mode) {
    case MainWindow::BACK_TO_MANDELBROT:
        ui->pushButtonStart->setText("Zurück zum Apfelmänchen");
        break;

    case MainWindow::CALC_JULIA:
        ui->pushButtonStart->setText("Zugehörige Julia-Menge berechnen");
        break;

    case MainWindow::CALC_JULIA_SETTINGS:
        ui->pushButtonStart->setText("Zugehörige Julia-Menge berechnen");
        break;

    case MainWindow::APPLY_SETTINGS_AND_ZOOM:
        ui->pushButtonStart->setText("Vergrößern mit neuen Einstellungen");
        break;

    case MainWindow::ZOOM_TO_SELECTED:
        this->op_mode = ZOOM_TO_SELECTED;
        break;

    case MainWindow::RESET:
    case MainWindow::REFRESH:
        ui->pushButtonStart->setText("Bild neu laden");
        break;

    case MainWindow::APPLY_SETTINGS:
        ui->pushButtonStart->setText("Einstellungen anwenden");
        break;

    default:
        break;
    }
}


void MainWindow::endRefresh(bool appendToListHistory)
{
    this->state = STATE::STOPED;
    ui->pushButtonStart->setText("Neu Laden");
    this->setOperationMode(OP_MODE::RESET);
    this->zoomRect.hide();

    ui->progressBar->setEnabled(false);
    ui->pushButtonSaveImg->setEnabled(true);
    ui->frameButtons->setEnabled(true);

    if(settingsList.length() > 1)
        ui->pushButton_back->setEnabled(true);
    ui->pushButton_vor->setDisabled(true);

    ui->lineEditScaleAbs->setText("1: " + QString::number((double)currentImg.scale()));
    if(currentImg.isMandelbrotSet) {
        ui->label_julia_c->setText("-");
        ui->label_julia_c_im->setText("-");
    } else {
        ui->label_julia_c->setText(QString::number( (double)(currentImg.juliaStart_real) )) ;
        ui->label_julia_c_im->setText(QString::number( -(double)(currentImg.juliaStart_img) ) + " i");
    }

    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->bmRe->setText(QString::number(this->currentImg.gaus_mid_re));
    ui->bmIm->setText(QString::number(this->currentImg.gaus_mid_im) + " i");


    afterColoring(currentImg);
    updateImage();

    if(ui->radioButtonAutoScroll->isChecked()) {
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum() / 2);
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->maximum() / 2);
    }

    if(appendToListHistory) {
        auto i = new QListWidgetItem( QString(currentImg.isMandelbrotSet ? "M" : "J" ) + ": Re(" + QString::number(this->currentImg.gaus_mid_re) +
                                     ") Im(" +  QString::number( - this->currentImg.gaus_mid_im ) + " i)");
        i->setIcon(QIcon(QPixmap::fromImage(*this->currentImg.image).scaled(QSize(256, 256), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::FastTransformation)));
        i->setData(187, currentImg.id);

        ui->listWidgetHistory->addItem(i);
        ui->listWidgetHistory->setCurrentItem(i);
    }

    this->updateUiWithImageSetting( currentImg );

    ui->widget->setDisabled(false);
}


void MainWindow::updateImage()
{
    if(currentImg.image)
        ui->imageView->setImage(*currentImg.image);
    this->update();
}

bool MainWindow::checkForFinished()
{
    if(this->ui->progressBar->value() == this->ui->progressBar->maximum() ) {
        for ( ssize_t i = 0; i < this->tworkers.length(); i++ ) {
            if (this->tworkers.at(i)->isFinished()) {
                auto tmp = this->tworkers.takeAt(i--);
                delete tmp;
            }
        }
        return this->tworkers.length() == 0;
    }
    return false;
}

void MainWindow::stopThreads()
{
    for ( ssize_t i = 0; i <  this->tworkers.length(); i++ ) {
        if (this->tworkers.at(i)->isRunning()) {
            this->tworkers.at(i)->quit();
            if(!this->tworkers.at(i)->wait(100)) {
                this->tworkers.at(i)->terminate();
            }
        }
    }

    for ( ssize_t i = 0; i <  this->tworkers.length(); i++ ) {
        auto tmp = this->tworkers.takeAt(i);
        delete tmp;
        i--;
    }
}


void MainWindow::on_pushButtonStart_clicked()
{
    zustandWechseln("BUTTON_REFRESH_AND_STOP");
}

#include<complex>

void MainWindow::finishedLine(QList<Pixel> *list)
{
    for( ssize_t l = 0; l < list->size(); l++) {
        currentImg.painter->setPen(list->at(l).color);

        ssize_t x = list->at(l).c_x,
                y = list->at(l).c_y;
        currentImg.painter->drawPoint(x, y);

        if(x >= 0 && x < currentImg.img_w && y >= 0 && y < currentImg.img_h) {
            currentImg.setIterationCountAt(x, y, list->at(l).iters);
            currentImg.iterations_normal[x][y] = list->at(l).normalized_iters;
            //Histogramm
            currentImg.addNumIterationsPerPixel(list->at(l).iters);
        }

    }
    this->ui->progressBar->setValue(this->ui->progressBar->value() + 1);
    delete list;
    imgUpdateNeeded = true;
}



#include <QFile>

void MainWindow::threadFinished()
{
//    WorkerThread * sender_obj = qobject_cast<WorkerThread*>(QObject::sender());
//    this->tworkers.removeAll(sender_obj);
//    if(sender_obj)
//        delete sender_obj;
}

void MainWindow::on_pushButtonSaveImg_clicked()
{
    QString info = QString(currentImg.isMandelbrotSet ? "Mandelbrot-Menge" : "Julia-Menge")
            + " Re(" + QString::number(this->currentImg.gaus_mid_re) + ") Im(" + QString::number(this->currentImg.gaus_mid_im) + ") Scale(1 zu " + QString::number(currentImg.scale()) + ")";
    QString filename = QFileDialog::getSaveFileName(this, "Bild speichern - Dateiendung nach gewünschtem Format ändern. Standart: .png", "", "Images (*.png *.xpm *.jpg)");

    if( ! filename.isEmpty()) {
        currentImg.image->setText("Description", info);
        filename = filename + ((!filename.contains(".")) ? ".png" : "");
        filename.insert(filename.lastIndexOf("."), " - " + info);

        if( ! currentImg.image->save(filename))
            QMessageBox::warning(this, "Speichern fehlgeschlagen!", "Das Bild '" + filename + "' konnte nicht gespeichert werden! Haben sie die Dateiendung vergessen?");
        else
            this->ui->statusbar->showMessage("Bild {Re(" + QString::number(this->currentImg.gaus_mid_re) + ") Im(" + QString::number(this->currentImg.gaus_mid_im) + ") 1:"
                                             +  QString::number(currentImg.scale()) + " [" + QString::number(currentImg.img_w) + "x" + QString::number(currentImg.img_h) + "]} gespeichert unter '" + filename + "'." , 5000);
    } else {
        this->ui->statusbar->showMessage("Kein Bild gespeichert!", 1000);
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    zustandWechseln("mousePressEvent", "", QPoint(), event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    zustandWechseln("DOPPELKLICK_IN_BILD", "", QPoint(), event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    zustandWechseln("mouseReleaseEvent", "", QPoint(), event);
}

void MainWindow::mouse_move_in_img(QPoint pos)
{
    zustandWechseln("mouseMoveEvent", "", pos);
}

void MainWindow::on_pushButton_vor_clicked()
{
    zustandWechseln("VOR");
}

void MainWindow::paintEvent(QPaintEvent *)
{
    switch (state) {
    case MainWindow::STOPED:

        if(zoomRect.isShown() || zahlenfolge.isShown() || this->ui->radioButtonKoords->isChecked()) {
            QImage img = *currentImg.image;
            QPainter painter(&img);

            if(zoomRect.isShown()) {
                if(!( currentImg.isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 1)) {
                    painter.setPen(Qt::black);
                    painter.drawRect(zoomRect.getRect());
                    painter.setPen(Qt::white);
                    //                painter.drawRect(QRect(x_left_corner - 1, y_left_corner - 1, w + 2, h + 2));
                    painter.drawRect(zoomRect.getRect().adjusted(-1, -1, +1, +1));
                } else {
                    painter.setPen(Qt::black);
                    painter.drawEllipse(zoomRect.getMousePos().rountToQPoint(), 4, 4);
                    painter.setPen(Qt::white);
                    painter.drawEllipse(zoomRect.getMousePos().rountToQPoint(), 5, 5);
                    painter.drawPoint(zoomRect.getMousePos().rountToQPoint());

                }

            }

            if(zahlenfolge.isShown()) {
                painter.setPen(QColor(ui->comboBox_color_zahlenfolge->currentText()));
                if( ui->comboBox_drawStyle_zahlenfolge->currentIndex() == 0) {
                    painter.drawPoints(zahlenfolge.getZahlenfolge().data(), zahlenfolge.getZahlenfolge().length());
                } else {
                    auto list = zahlenfolge.getZahlenfolge();
                    QPoint last = QPoint();
                    if(list.length() > 0) {
                        painter.drawPoint(list.first());
                        last = list.first();
                    }
                    for( unsigned i = 1; i < list.length(); i++ ) {
                        painter.setPen(QColor(ui->comboBox_color_zahlenfolge->currentText()));
                        painter.drawLine(last, list.at(i));
                        if(last != list.at(i))
                            last = list.at(i);
//                        else
//                            break;
                        if(ui->comboBox_drawStyle_zahlenfolge->currentIndex() == 2)
                            painter.drawEllipse(last, 3, 3);

                    }


                }
            }


            // optimieren -> nur sichtbares zeug berechnen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(this->ui->radioButtonKoords->isChecked()) {
                painter.setPen(QColor(ui->comboBoxColorKoordSystem->currentText()));
                auto xA = currentImg.xAchse;
                auto yA = currentImg.yAchse;

                //Achsen
                painter.drawLine(xA);
                painter.drawLine(yA);

                //Pfeile
                painter.drawLine(xA.p2(), xA.p2() + QPoint(-5, -5));
                painter.drawLine(xA.p2(), xA.p2() + QPoint(-5, 5));

                painter.drawLine(yA.p1(), yA.p1() - QPoint(-5, -5));
                painter.drawLine(yA.p1(), yA.p1() - QPoint(5, -5));

                if(this->ui->radioButtonMitBeschriftungen->isChecked()) {
                    //Beschriftung
                    int partsX = xA.dx() / ui->spinBoxBeschriftungen->value();
                    for(int i = 0; i < ui->spinBoxBeschriftungen->value(); i++) {
                        painter.drawLine(xA.p1() + QPoint(partsX * i, -5), xA.p1() + QPoint(partsX * i, 5));

                        painter.drawText(xA.p1() + QPoint(partsX * i - 10, 20),
                                         QString::number((double)currentImg.mapImgPosReToGaus(xA.p1().x() + partsX * i)));
                    }

                    int partsY = yA.dy() / ui->spinBoxBeschriftungenY->value();
                    for(int i = 1; i <= ui->spinBoxBeschriftungenY->value(); i++) {
                        painter.drawLine(yA.p1() + QPoint(-5, partsY * i), yA.p1() + QPoint(5, partsY * i));
                        painter.drawText(yA.p1() + QPoint( -25, partsY * i),
                                         QString::number( - (double)currentImg.mapImgPosImToGaus(yA.p1().y() + partsY * i)));
                    }
                }

            }

//            painter.end();
//            QPainter painter2(ui->labelFraktal);
//            this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));
//            painter2.drawImage(0, 0, img);
//            painter2.end();
            //this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));
            ui->imageView->setImage(img);

        }

        break;
    case MainWindow::RUNNING:
        break;

    }


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event << " --> ";

}

void MainWindow::keyReleaseEvent(QKeyEvent *)
{
}


ImageSetting::ImageSetting()
{
    has_hue = false;
}

ImageSetting::ImageSetting(int id, double scale, double re, double im)
    : gaus_mid_re(re), gaus_mid_im(im), scaleValue(scale), has_hue(false), hue(nullptr)
{
    this->id = id;
}

void ImageSetting::init(size_t img_w, size_t img_h, size_t maxIterations, bool isMandelbrotSet)
{
    this->img_w = img_w;
    this->img_h = img_h;
    this->maxIterations = maxIterations;
    this->isMandelbrotSet = isMandelbrotSet;
//    isStart = false;

    this->image = new QImage(img_w, img_h, QImage::Format_ARGB32_Premultiplied);
    this->painter = new QPainter(this->image);

    this->re_verscheibung = img_w / 2;
    this->im_verscheibung = img_h / 2;

    this->iterations = new size_t*[img_w];
    for( size_t i = 0; i < img_w; i++) {
        this->iterations[i] = new size_t[img_h];
        memset(this->iterations[i], 0, sizeof(size_t) * img_h );
    }

    this->iterations_normal = new double*[img_w];
    for( size_t i = 0; i < img_w; i++) {
        this->iterations_normal[i] = new double[img_h];
        memset(this->iterations_normal[i], 0, sizeof(double) * img_h );
    }

    // For Histogram
    NumIterationsPerPixel = new size_t[maxIterations + 2];
    memset(NumIterationsPerPixel, 0, sizeof(size_t) * (maxIterations + 2) );

    QPoint startX = QPoint(10, img_h / 2);
    QPoint endeX = QPoint (img_w - 10, startX.y());

    QPoint startY = QPoint (img_w / 2, 10);
    QPoint endeY = QPoint (startY.x(), img_h - 10 );

    this->xAchse = QLine(startX, endeX);
    this->yAchse = QLine(startY, endeY);

}

void ::ImageSetting::setIterationCountAt(ssize_t x, ssize_t y, size_t iterations)
{
    this->iterations[x][y] = iterations;
}

void ImageSetting::setColorSettings(int palette, bool normalized, double logEscape, bool fixedColor, QString fixFraktalColor, bool inverted, double escape_radius)
{
    this->normalized = normalized;
    this->logEscape = logEscape;
    this->fixedColor = fixedColor;
    this->fixFraktalColor = fixFraktalColor;
    this->inverted = inverted;
    this->escape_radius = escape_radius;
    this->palette = palette;

}

size_t ImageSetting::getIterationCountAt(QPoint pos)
{
    return this->getIterationCountAt(pos.x(), pos.y());
}

size_t ImageSetting::getIterationCountAt(ssize_t x, ssize_t y)
{
    if(x >= 0 && x < this->img_w && y >= 0 && y < this->img_h)
        return this->iterations[x][y];
    else
        return -1;
}

double ImageSetting::mapImgPosReToGaus(ssize_t img_pos_re)
{
    // mid punkt img == mid punkt gaus -->
    // x verscheibung == mid_punkt_img
    return (((double)img_pos_re - re_verscheibung ) / scale()) + gaus_mid_re;
}

long double ImageSetting::mapImgPosReToGausLong(ssize_t img_pos_re)
{
    return (((long double)img_pos_re - (long double)re_verscheibung ) / (long double)scale()) + (long double)gaus_mid_re;
}

ssize_t ImageSetting::mapGausReToImgPos(double gaus_re)
{
    return (gaus_re - gaus_mid_re) * scale() + re_verscheibung;
}

double ImageSetting::mapImgPosImToGaus(ssize_t img_pos_im)
{
    return (((double)img_pos_im - im_verscheibung ) / scale()) + gaus_mid_im;
}

long double ImageSetting::mapImgPosImToGausLong(ssize_t img_pos_im)
{
    return (((long double)img_pos_im - (long double)im_verscheibung ) / (long double)scale()) + (long double)gaus_mid_im;
}

ssize_t ImageSetting::mapGausImToImgPos(double gaus_im)
{
    return (gaus_im - gaus_mid_im) * scale() + im_verscheibung;
}

double ImageSetting::scale()
{
    return (double)scaleValue;
}



void MainWindow::on_spinBoxMaxIterations_valueChanged(int arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_comboBox_palette_currentIndexChanged(int index)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);

    ui->stackedWidgetExtraSettings->setVisible(false);
    ui->spinBoxFarbWechselIntervall->setVisible(false);
    ui->label_farbwechselintervall->setVisible(false);
    ui->radioButton_normalized->setEnabled(true);


    switch(index) {
    case 2:
        ui->spinBoxFarbWechselIntervall->setVisible(true);
        ui->label_farbwechselintervall->setVisible(true);
        ui->stackedWidgetExtraSettings->setCurrentIndex(4);
        ui->stackedWidgetExtraSettings->setVisible(true);
        ui->radioButton_normalized->setEnabled(false);
        break;
    case 1:
        ui->stackedWidgetExtraSettings->setCurrentIndex(4);
        ui->stackedWidgetExtraSettings->setVisible(true);
        ui->radioButton_normalized->setEnabled(false);
        break;

    case 3:
        ui->radioButton_normalized->setEnabled(false);
        break;

    case 4:
    case 5:
        ui->stackedWidgetExtraSettings->setCurrentIndex(3);
        ui->stackedWidgetExtraSettings->setVisible(true);
        break;

    case 6:
        ui->radioButton_normalized->setEnabled(false);
    break;
    }


}


void MainWindow::on_radioButton_normalized_toggled(bool checked)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_radioButton_invert_toggled(bool checked)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_spinBoxW_valueChanged(int arg1)
{
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
    this->updateImage();
}


void MainWindow::on_spinBoxH_valueChanged(int arg1)
{
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
    this->updateImage();
}


void MainWindow::on_comboBox_background_color_currentIndexChanged(int index)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_groupBoxMandelFarbe_toggled(bool arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_comboBoxMandelColor_currentIndexChanged(int index)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_comboBox_precession_currentIndexChanged(int index)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_doubleSpinBoxEscapeR_valueChanged(double arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_radioButton_reload_at_back_toggled(bool checked)
{
//no setting
}

#include <limits>
#include <cmath>

#include <chrono>

void MainWindow::ZahlenFolge::setZahlenfolge(Point c_p, ImageSetting s)
{
    if(this->isShown())
        this->removeZahlenfolge();

    double z_re, z_im, c_re, c_im;


    if ( s.isMandelbrotSet ) {
        z_re = 0;
        z_im = 0;
        c_re = s.mapImgPosReToGaus(c_p.x());
        c_im = s.mapImgPosImToGaus(c_p.y());

    } else {
        z_re = s.mapImgPosReToGaus(c_p.x());
        z_im = s.mapImgPosImToGaus(c_p.y());
        c_re = s.juliaStart_real;
        c_im = s.juliaStart_img;
    }
//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    QPoint addPoint;
        for(size_t i = 0; i < s.maxIterations &&i < 10000 && std::abs(addPoint.x()) < s.img_w && std::abs(addPoint.y()) < s.img_h; i++) {
            const double z_re2 = (z_re * z_re) - (z_im * z_im) + c_re;
            const double z_im2 = (2.0 * z_re * z_im) + c_im;
            addPoint = QPoint(s.mapGausReToImgPos(z_re2), s.mapGausImToImgPos(z_im2));
            this->zahlenfolge.push_back(addPoint);

            i++;
            z_re = (z_re2 * z_re2) - (z_im2 * z_im2) + c_re;
            z_im = (2.0 * z_re2 * z_im2) + c_im;
            addPoint = QPoint(s.mapGausReToImgPos(z_re), s.mapGausImToImgPos(z_im));
            this->zahlenfolge.push_back(addPoint);
        }
//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    this->show = true;
}

void MainWindow::ZahlenFolge::removeZahlenfolge()
{
    this->zahlenfolge.clear();
    this->show = false;
}

QList<QPoint> MainWindow::ZahlenFolge::getZahlenfolge()
{
    return this->zahlenfolge;
}

bool MainWindow::ZahlenFolge::isShown()
{
    return this->show;
}

void MainWindow::on_pushButton_scale_plus_clicked()
{
//    auto img = ui->labelFraktal->pixmap();

//    img = img.scaled( (double)img.width() * 1.1, (double)img.height() * 1.1, Qt::AspectRatioMode::KeepAspectRatio,  Qt::TransformationMode::SmoothTransformation);
//    if( ! img.isNull())
//        this->ui->labelFraktal->setPixmap(img);
}


void MainWindow::on_pushButton_scale_minus_clicked()
{
//    auto img = ui->labelFraktal->pixmap();

//    img = img.scaled( (double)img.width() * 0.9, (double)img.height() * 0.9, Qt::AspectRatioMode::KeepAspectRatio,  Qt::TransformationMode::SmoothTransformation);
//    if( ! img.isNull())
//        this->ui->labelFraktal->setPixmap(img);
}


void MainWindow::ZoomRect::updateRectPos(Point p)
{
    this->mousePos = p;
    auto left_top = p.rountToQPoint() - QPoint(rect.width() / 2, rect.height() / 2);
    this->rect.moveTopLeft(left_top);
    //    this->rect.setRect(left_top.x(), left_top.y(), rect.width(), rect.height());

}



MainWindow::ZoomRect::ZoomRect()
{
    is_shown = false;
    right_mouse_press = false;
}

void MainWindow::ZoomRect::setMousePressState(bool isPressed)
{
    this->right_mouse_press = isPressed;
}

void MainWindow::ZoomRect::show()
{
    is_shown = true;
}

void MainWindow::ZoomRect::hide()
{
    is_shown = false;
}

void MainWindow::ZoomRect::updateRectSize(size_t winW, size_t winH, double scale)
{
    if(scale >= 1) {
        this->rect.setSize(QSize(winW / scale, (double)winH / scale));
        this->updateRectPos(this->mousePos);
    }
}

QRect MainWindow::ZoomRect::getRect()
{
    return rect;
}

Point MainWindow::ZoomRect::getMousePos()
{
    return mousePos;
}

bool MainWindow::ZoomRect::isShown()
{
    return is_shown;
}

bool MainWindow::ZoomRect::rightMouseIsPressed()
{
    return right_mouse_press;
}

Point::Point()
{
    X = 0;
    Y = 0;
}

Point::Point(long long X, long long Y)
    : X(X), Y(Y)
{

}

QPoint Point::rountToQPoint()
{
    return QPoint(x(), y());
}

void Point::setX(long long newX)
{
    X = newX;
}

long long Point::y() const
{
    return Y;
}

void Point::setY(long long newY)
{
    Y = newY;
}

long long Point::x() const
{
    return X;
}


void ImageSetting::cleanUP()
{
    if(painter) {
        delete painter;
        painter = nullptr;
    }
    if(image) {
        delete image;
        image = nullptr;
    }

    for(int i = 0; i < img_w; i++ )
        delete[] iterations[i];
    delete [] iterations;
    iterations = nullptr;

    for(int i = 0; i < img_w; i++ )
        delete[] iterations_normal[i];
    delete [] iterations_normal;
    iterations_normal = nullptr;

    delete[] NumIterationsPerPixel;
    NumIterationsPerPixel = nullptr;

    if(hueIsSetted()) {
        for(int i = 0; i < img_w; i++ )
            delete[] hue[i];
        delete [] hue;
        hue = nullptr;
        this->has_hue = false;
    }
}


void MainWindow::on_spinBox_zoom_valueChanged(double arg1)
{
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
    this->updateImage();
}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    double multiplikator = 1.0;

    switch (index) {
    case 0: // auto %
        multiplikator = ui->imageView->devicePixelRatio();
        break;
    case 1:
        multiplikator = 4.00;
        break;
    case 2: // 50 % hdpi
        multiplikator = 2.00;
        break;
    case 3: // 100 % dpi
        multiplikator = 1.00;
        break;
    case 4:
        multiplikator = 0.50;
        break;
    case 5:
        multiplikator = 0.25;
        break;
    default:
        multiplikator = ui->imageView->devicePixelRatio();
        break;
    }

    ui->imageView->setHdpiScale(multiplikator);
    this->updateImage();
}





void MainWindow::on_pushButton_rm_history_clicked()
{

    for(int i = 1; i < settingsList.length() - 1; i++) {
        auto s = settingsList.takeAt(i--);
        s.cleanUP();
    }

    this->currentImg = settingsList.last();
    if(ui->radioButton_reload_at_back->isChecked())
        this->startRefresh(settingsList.last());
    else {
        this->endRefresh(false);
        updateImage();
    }
    for(int i = 1; i < ui->listWidgetHistory->count() - 1; i++)
        delete ui->listWidgetHistory->takeItem(i--);
}


void MainWindow::on_listWidgetHistory_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->data(187).toInt();
    for(int i = 0; i <  settingsList.length(); i++) {
        if(settingsList.at(i).id == id) {

            this->currentImg = settingsList.at(i);
            if(ui->radioButton_reload_at_back->isChecked())
                this->startRefresh(settingsList.at(i));
            else {
                this->endRefresh(false);
                updateImage();
            }
            ui->listWidgetHistory->setCurrentItem(item);


            if(i < settingsList.length() - 1)
                ui->pushButton_vor->setEnabled(true);
            else
                ui->pushButton_vor->setEnabled(false);

            if(i > 0)
                ui->pushButton_back->setEnabled(true);
            else
                ui->pushButton_back->setEnabled(false);

            return;
        }
    }



}


void MainWindow::on_re_valueChanged(double arg1)
{
    if(ignoreXPosEdited) {
        ignoreXPosEdited = false;
        return;
    }
    this->zoomRect.updateRectPos(Point(currentImg.mapGausReToImgPos(arg1), zoomRect.getMousePos().y()));
    this->zoomRect.show();
    this->setOperationMode(OP_MODE::ZOOM_TO_SELECTED);

    updateImage();
}


void MainWindow::on_im_valueChanged(double arg1)
{
    if(ignoreYPosEdited) {
        ignoreYPosEdited = false;
        return;
    }

    this->zoomRect.updateRectPos(Point(zoomRect.getMousePos().x(), currentImg.mapGausImToImgPos(-1* arg1)));
    this->zoomRect.show();
    this->setOperationMode(OP_MODE::ZOOM_TO_SELECTED);

    updateImage();
}

#include <QClipboard>

void MainWindow::on_pushButton_copy_clicked()
{
    QApplication::clipboard()->setImage(*this->currentImg.image);
    this->ui->statusbar->showMessage("Bild {Re(" + QString::number(this->currentImg.gaus_mid_re) + ") Im(" + QString::number(this->currentImg.gaus_mid_im) + ") 1:"
                                     +  QString::number(currentImg.scale()) + " [" + QString::number(currentImg.img_w) + "x" + QString::number(currentImg.img_h) + "]} in Zwischenablage kopiert." , 4000);
}


void MainWindow::on_pushButton_back_clicked()
{
    zustandWechseln("ZURÜCK");
}


void MainWindow::on_pushButtonHome_clicked()
{
    zustandWechseln("HOME");
}

