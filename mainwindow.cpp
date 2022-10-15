#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QTime>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->startTimer(20);
    i= new ImageView();
    i->show();

}

MainWindow::~MainWindow()
{
    stopThreads();

//    this->painter->end();
//    delete this->painter;
//    this->painter = nullptr;

    delete ui;
    ui = nullptr;

//    delete image;
//    image = nullptr;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    zustandWechseln("TIMER");
}


void MainWindow::zustandWechseln(QString aktion, QString, size_t, QMouseEvent *m_event)
{
    if(aktion != "TIMER" && aktion != "mouseMoveEvent")
        qDebug() << "zustandWechseln "<< aktion;

    switch (this->state) {
    case MainWindow::INIT:
        if(aktion == "TIMER") {
            this->loadInitImg();
        }

        break;
    case MainWindow::STOPED:

        if(aktion == "TIMER") {

        } else if(aktion == "BUTTON_REFRESH_AND_STOP") {
            qDebug() << this->op_mode;

            if(this->op_mode == OP_MODE::REFRESH) {
                //Aktualiseren
                this->startRefresh(settings.last());

            // wenn: einstellungenverändert == true:
            } else if(this->op_mode == OP_MODE::APPLY_SETTINGS || this->op_mode == OP_MODE::ZOOM_TO_SELECTED || this->op_mode == OP_MODE::APPLY_SETTINGS_AND_ZOOM) {
                //Zoomen
                SETTING newSetting = getNewScaledSetting(settings.last());
                this->startRefresh(newSetting, true);
                this->ui->pushButton_2->setDisabled(false);

            }

        } else if( aktion == "DOPPELKLICK_IN_BILD" && m_event) {
            auto pos = this->ui->labelFraktal->mapFrom(this, m_event->pos());
            if(m_event->button() == Qt::LeftButton && pos.x() >= 0 && pos.x() < settings.last().img_w && pos.y() >= 0 && pos.y() < settings.last().img_h) {
                SETTING newSetting = getNewScaledSetting(settings.last());
                this->startRefresh(newSetting, true);
                this->ui->pushButton_2->setDisabled(false);

            }
        } else if(aktion == "mouseReleaseEvent" && m_event) {
            if(m_event->button() == Qt::LeftButton) {
                // no update in paintevent!! --> no move!!
                this->mousePress.isPressed = false;
                this->update();
            }

        } else if(aktion == "mouseMoveEvent" && m_event) {

            auto qpos = this->ui->labelFraktal->mapFrom(this, m_event->pos());
            auto pos = Point(qpos.x(), qpos.y());

            qDebug() << qpos;
            if(pos.x() >= 0 && pos.x() < settings.last().img_w && pos.y() >= 0 && pos.y() < settings.last().img_h) {
                this->mousePress.updateKoords( pos, settings.last().x_verschiebung, settings.last().y_verschiebung  );
                if(this->mousePress.isPressed) {
                    this->setOperationMode(OP_MODE::ZOOM_TO_SELECTED);
                    this->update();
                }
                ui->label_iterations->setText(QString::number(settings.last().getIterationCountAt(qpos)));

                ui->re->setText(QString::number((double(this->mousePress.koord.x() / settings.last().scale))));
                ui->img->setText(QString::number( - (double(this->mousePress.koord.y() / settings.last().scale)))  + "i");


                if( ui->radioButton_zahelnfolge_bei_mousemove->isChecked() && zahlenfolge.isShown() ) {
                    this->zahlenfolge.setZahlenfolge(pos, settings.last());
                    this->update();
                }

            } else {
                ui->label_iterations->setText("-");
                ui->re->setText("-");
                ui->img->setText("-");
            }

        } else if(aktion == "mousePressEvent"  && m_event) {

            auto qpos = this->ui->labelFraktal->mapFrom(this, m_event->pos());
            auto pos = Point(qpos.x(), qpos.y());

            if(pos.x() >= 0 && pos.x() < settings.last().img_w && pos.y() >= 0 && pos.y() < settings.last().img_h) {
                if(m_event->button() == Qt::LeftButton) {
                    mousePress.setPressed();
                    this->mousePress.updateKoords(pos , settings.last().x_verschiebung, settings.last().y_verschiebung );
                    this->setOperationMode(OP_MODE::ZOOM_TO_SELECTED);

                    ui->label_iterations->setText(QString::number(settings.last().getIterationCountAt(qpos)));
                    ui->re->setText(QString::number((double(this->mousePress.koord.x() / settings.last().scale))));
                    ui->img->setText( QString::number( - (double(this->mousePress.koord.y() / settings.last().scale)))  + "i");

                    if(zahlenfolge.isShown())
                        this->zahlenfolge.removeZahlenfolge();
                } else  if(m_event->button() == Qt::RightButton) {
                    this->zahlenfolge.setZahlenfolge(pos, settings.last());
                }

                this->update();
            } else {
                this->setOperationMode(OP_MODE::REFRESH);

                this->mousePress.isPressed = false;

                ui->re->setText("-");
                ui->img->setText("-");
                ui->label_iterations->setText("-");
                this->zahlenfolge.removeZahlenfolge();

                updateImage();
            }


        } else if(aktion == "ZURÜCK") {
            if(settings.length() > 1) {
                auto s = settings.takeLast();
                delete s.painter;
                delete s.image;
            }
            if(ui->radioButton_reload_at_back->isChecked())
                this->startRefresh(settings.last());
            else {
                this->ui->labelFraktal->setFixedSize( settings.last().img_w, settings.last().img_h);
                this->endRefresh();
                updateImage();
            }

        } else if(aktion == "HOME") {
            while(settings.length() > 1) {
                auto s = settings.takeLast();
                delete s.painter;
                delete s.image;
            }
            if(ui->radioButton_reload_at_back->isChecked())
                this->startRefresh(settings.last());
            else {
                this->ui->labelFraktal->setFixedSize( settings.last().img_w, settings.last().img_h);
                this->endRefresh();
                updateImage();
            }
        }


        break;
    case MainWindow::RUNNING:
        if(aktion == "TIMER") {
            if(imgUpdateNeeded)
                updateImage();
            if(checkForFinished())
                endRefresh();

        } else if(aktion == "BUTTON_REFRESH_AND_STOP") {
            this->stopThreads();
            this->endRefresh();
        }

        break;

    }
}

MainWindow::SETTING MainWindow::getNewScaledSetting(SETTING lastSet)
{
    //es wird davon ausgegangen, dass diese Funktion dann aufgerufen wird, wenn mousePress gesetzt ist! Wenn nicht, dann sollte lastSetGeladen werden.
    // entweder zoomfeld gesetzt und davon mitte, egal ob doppelklick oder button, oder reines neu laden button!!

    double relativeZoom = ui->spinBox_zoom->value();
    if(ui->spinBox_zoom->value() == 0.0)
        ui->spinBox_zoom->setValue( (relativeZoom = 1.0) );


    // Zoom midposize_t --> new scaling!!!!
    size_t midx =0 ;
    size_t midy =0 ;
    long double nscale = 0;

    if(relativeZoom > 0) {
        nscale = lastSet.scale * (long double) relativeZoom;
        midx   = (this->mousePress.point.x() + lastSet.x_verschiebung) * relativeZoom;
        midy   = (this->mousePress.point.y() + lastSet.y_verschiebung) * relativeZoom;
    } else {
        nscale  = -lastSet.scale / (long double) relativeZoom;
        midx    = -(this->mousePress.point.x() + lastSet.x_verschiebung) / relativeZoom;
        midy    = -(this->mousePress.point.y() + lastSet.y_verschiebung) / relativeZoom;
    }

    SETTING s(nscale, Point(midx, midy));
    s.init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBoxMaxIterations->value());
    return s;
}

long double MainWindow::getNewScale(SETTING lastSet)
{
    //    check if max is reached!!!


}

void MainWindow::setColor(QPainter * mpainter, size_t iters, std::complex<long double> z_n)
{
    if(!mpainter)
        return;

    double n;
    double alpha;
    double v = 0;

    // normalizing
    if(iters != 0) {
        if(this->ui->radioButton_normalized->isChecked())
            n = abs(iters + (( this->logEscape - log(log(abs(z_n))) ) / this->log2));
        else
            n = iters;
    }

    if((iters == 0) && ui->groupBoxMandelFarbe->isChecked()) {
        mpainter->setPen(QColor(ui->comboBoxMandelColor->currentText()));
    } else {
        //coloring
        switch (ui->comboBox_palette->currentIndex()) {
        case 0:
            alpha = (iters == 0) ? 1.0 /*Mandelbrot-Set-Alpha*/ : (double)((int)(n*10.0) % 255) / 255.0; /*Outside*/
            mpainter->setPen(QColor::fromRgbF(0, 0, 0, ui->radioButton_invert->isChecked() ? 1.0 - alpha : alpha ));
            break;
        case 1:
            v = (iters == 0) ? 255.0 /*Mandelbrot-Set-Alpha*/ : ((double)((int)(n*3.0) % 255) / 255.0);
            v = ui->radioButton_invert->isChecked() ? 255.0 - v : v;
            mpainter->setPen(QColor::fromRgbF(1, v, v, 1 ));
            break;
        case 3:
            alpha = (iters == 0) ? 1.0 /*Mandelbrot-Set-Alpha*/ : (double)((int)(n) % ui->spinBoxMaxIterations->value()) / ((double)ui->spinBoxMaxIterations->value()); /*Outside*/
            mpainter->setPen(QColor::fromRgbF(0, 0, 0, ui->radioButton_invert->isChecked() ? 1.0 - alpha : alpha ));
            break;
        case 4:
            if(iters == 0) {
                mpainter->setPen(QColor(Qt::black));
            } else {
                // hsv = [powf((i / max) * 360, 1.5) % 360, 100, (i / max) * 100]
                QColor c = QColor::fromHsv( size_t(pow( (n / (double)settings.last().maxIterations) * 358 , 1.5)) % 358, 100, (n / (double)settings.last().maxIterations) * 250);
                mpainter->setPen(c);
            }
            break;
        case 5:
            if(iters == 0) {
                mpainter->setPen(QColor(Qt::black));
            } else {
                mpainter->setPen(QColor::fromHsv(int( (358.0 * n) / (double)settings.last().maxIterations ), 255, 255));
            }
            break;
        default:
            alpha = (iters == 0) ? 1.0 /*Mandelbrot-Set-Alpha*/ : 0.0; /*Outside*/
            mpainter->setPen(QColor::fromRgbF(0, 0, 0, ui->radioButton_invert->isChecked() ? 1.0 - alpha : alpha ));
            break;
        }
    }
}

#include <algorithm>


void MainWindow::afterColoring(SETTING)
{
//    size_t * NumIterationsPerPixel = new size_t[set.maxIterations];
//    memset(NumIterationsPerPixel, 0, sizeof(int) * set.maxIterations );

//    for(size_t x = 0; x < set.img_w; x++) {
//        for(size_t y = 0; y < set.img_h; y++) {
//            size_t i = set.getIterationCountAt(x, y);
//            if(i >= 0 && i < set.maxIterations)
//                NumIterationsPerPixel[i]++;
//        }
//    }

//    size_t total = 0;
//    for (size_t i = 0; i < set.maxIterations; i++)
//        total += NumIterationsPerPixel[i];

//    auto * img = new QImage(set.img_w, set.img_h, QImage::Format_ARGB32_Premultiplied);
//    auto * p =  new QPainter(img);


//    for(size_t x = 0; x < set.img_w; x++) {
//        for(size_t y = 0; y < set.img_h; y++) {

//        size_t iteration= set.getIterationCountAt(x, y);
//        double hue = 0;
//        for (size_t i = 0; i < iteration; i++) {
//            hue += (double)NumIterationsPerPixel[i] / (double)total; /* Must be floating-posize_t division. */
//        }
////        qDebug() << "p("<<x<<"|"<<y<<"): i="<<iteration<< " ---> hue: " << size_t(hue*358.0) % 358;
////        QColor c = QColor::fromHsv(hue , 100, (iteration / (double)settings.last().maxIterations) * 250);
//        auto c = QColor::fromHsv(int(hue*360), 1, 1);
////        p->setPen(c);
////        p->drawPoint(x, y);

//        }

//    }
////    this->ui->labelFraktal->setPixmap(QPixmap::fromImage(*img));

//    delete[] NumIterationsPerPixel;
    //    return;
}

void MainWindow::setOperationMode(OP_MODE o)
{
    if(this->op_mode == OP_MODE::APPLY_SETTINGS_AND_ZOOM && o == OP_MODE::REFRESH)
        o = OP_MODE::APPLY_SETTINGS;
    else if(this->op_mode == OP_MODE::APPLY_SETTINGS && o == OP_MODE::ZOOM_TO_SELECTED)
        o = OP_MODE::APPLY_SETTINGS_AND_ZOOM;
    else if(this->op_mode == OP_MODE::ZOOM_TO_SELECTED && o == OP_MODE::APPLY_SETTINGS)
        o = OP_MODE::APPLY_SETTINGS_AND_ZOOM;
    else if(this->op_mode == OP_MODE::APPLY_SETTINGS && o == OP_MODE::REFRESH)
        return;
    else if(this->op_mode == OP_MODE::APPLY_SETTINGS_AND_ZOOM && (o == OP_MODE::ZOOM_TO_SELECTED || o == OP_MODE::APPLY_SETTINGS) )
        return;

    switch (o) {
    case MainWindow::APPLY_SETTINGS_AND_ZOOM:
        ui->pushButtonStart->setText("Vergrößern mit neuen Einstellungen");
        this->op_mode = APPLY_SETTINGS_AND_ZOOM;
        break;
    case MainWindow::ZOOM_TO_SELECTED:
        ui->pushButtonStart->setText("Vergrößern");
        this->op_mode = ZOOM_TO_SELECTED;
        break;

    case MainWindow::RESET:
    case MainWindow::REFRESH:
        ui->pushButtonStart->setText("Bild neu laden");
        this->op_mode = REFRESH;
        break;
    case MainWindow::APPLY_SETTINGS:
        ui->pushButtonStart->setText("Einstellungen anwenden");
        this->op_mode = APPLY_SETTINGS;
        break;
    default:
        break;
    }


}

void MainWindow::startRefresh(SETTING set, bool zoom)
{
    if(set.scale == 0) {
        perror("INVALID SETTING!");
        return;
    }

    //reset ui
    this->state = STATE::RUNNING;
    ui->widget->setDisabled(true);
    ui->pushButtonStart->setText("Abbrechen");
    ui->progressBar->setEnabled(true);
    ui->frameButtons->setEnabled(false);

    zahlenfolge.removeZahlenfolge();


    //calc work for each thread
    size_t tc = std::max(std::min( ui->spinBox_threads->value(), (int)set.img_w), 1);
    ssize_t parts = set.img_w / tc;

    //fill image with background color:
    set.painter->fillRect(set.image->rect(), QColor(ui->comboBox_background_color->currentText()));
    this->ui->labelFraktal->setFixedSize( set.img_w, set.img_h);
    this->updateImage();

    //reset progress bar
    this->ui->progressBar->setMaximum(set.img_w);
    this->ui->progressBar->setValue(0);

    //calc consts for normalised iteration count
    this->logEscape = log(log(ui->doubleSpinBoxEscapeR->value()));
    this->log2 = log(2);

    // Verschiebung so, dass ecke bei (0|0) --> verschiebung == echte ecke
    ssize_t x_left_corner = set.x_verschiebung;
    ssize_t y_left_corner = set.y_verschiebung;

    // wenn nicht nur neuladen --> add new settings to list
    if(zoom)
        settings.push_back(set);


    //start workers
    for( size_t tn = 0; tn < tc; tn++) {
        WorkerThread * wt = new WorkerThread(this);
        this->tworkers.append( wt );

        qDebug() << "ssstart with: " << x_left_corner;

        wt->startCalc(x_left_corner + parts * tn,
                      x_left_corner + parts * (tn + 1) + ( (tn + 1 == tc ) ? (set.img_w - parts * (tn + 1)) : 0), // wenn breite mit threadzahl nicht aufgeht -> letzter thread noch etwas breiter
                      parts * (tn + 1) + ( (tn + 1 == tc ) ? (set.img_w - parts * (tn + 1)) : 0),
                      y_left_corner,
                      y_left_corner + set.img_h,
                      ui->spinBoxMaxIterations->value(),
                      ui->doubleSpinBoxEscapeR->value(),
                      ((double long)1.0 / set.scale),
                      this->ui->comboBox_precession->currentIndex() == 0 ? WorkerThread::PRECESSION::DOUBLE : WorkerThread::PRECESSION::LONG_DOUBLE );

        connect(wt, SIGNAL(finishedLine(QList<WorkerThread::Pixel>*)), this, SLOT(finishedLine(QList<WorkerThread::Pixel>*)));
        connect(wt, SIGNAL(finished()), this, SLOT(threadFinished()));

        QTime dieTime= QTime::currentTime().addMSecs(20);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::endRefresh()
{
    this->state = STATE::STOPED;
    ui->widget->setDisabled(false);
    ui->pushButtonStart->setText("Neu Laden");
    this->setOperationMode(OP_MODE::RESET);
    ui->progressBar->setEnabled(false);
    ui->pushButtonSaveImg->setEnabled(true);
    ui->frameButtons->setEnabled(true);
    ui->lineEditScaleAbs->setText("1:" + QString::number((double)settings.last().scale));
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->bmRe->setText(QString::number( (double)(((long double) this->settings.last().midPoint.x()) / this->settings.last().scale) ));
    ui->bmIm->setText(QString::number( (double)(((long double) this->settings.last().midPoint.y()) / this->settings.last().scale) ) + "i");

    afterColoring(settings.last());
}

void MainWindow::loadInitImg()
{
    //später das zuletzt verwendete
//    SETTING newImage(150, Point(-80, 0));
    SETTING newImage(120, Point(-80, 0));
    newImage.init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBoxMaxIterations->value());
    newImage.painter->fillRect(newImage.image->rect(), QColor(ui->comboBox_background_color->currentText()));

    this->ui->labelFraktal->setPixmap(QPixmap::fromImage(*newImage.image));
    this->ui->labelFraktal->setFixedSize( ui->spinBoxW->value(), ui->spinBoxH->value());

    this->startRefresh(newImage, true);
}

void MainWindow::updateImage()
{
    if(settings.empty())
        return;
//    auto img = QPixmap::fromImage(*settings.last().image);
//    if( ! img.isNull())
//        this->ui->labelFraktal->setPixmap(img);
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

void MainWindow::finishedLine(QList<WorkerThread::Pixel> *list)
{
//    this->writequeque.push_back(list);
    for( ssize_t l = 0; l < list->size(); l++) {
        this->setColor(settings.last().painter, list->at(l).i, list->at(l).z_n);
        ssize_t x = list->at(l).c_x - settings.last().x_verschiebung,
            y = list->at(l).c_y -  settings.last().y_verschiebung;
        settings.last().painter->drawPoint(x, y);
        if(x >= 0 && x < this->settings.last().img_w && y >= 0 && y < this->settings.last().img_h)
            settings.last().setIterationCountAt(x, y, list->at(l).i);

    }
    //QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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
    QString filename = QFileDialog::getSaveFileName(this, "Bild speichern - Dateiendung nach gewünschtem Format ändern. Standart: .png", "", "Images (*.png *.xpm *.jpg)");
    if( ! filename.isEmpty())
        if( ! settings.last().image->save(filename + ((!filename.contains(".")) ? ".png" : "")) )
            QMessageBox::warning(this, "Speichern fehlgeschlagen!", "Das Bild konnte nicht gespeichert werden! Haben sie die Dateiendung vergessen?");
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    zustandWechseln("mousePressEvent", "", 0, event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    zustandWechseln("DOPPELKLICK_IN_BILD", "", 0, event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    zustandWechseln("mouseReleaseEvent", "", 0, event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    zustandWechseln("mouseMoveEvent", "", 0, event);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    switch (state) {
    case MainWindow::INIT:
        break;
    case MainWindow::STOPED:

        if(mousePress.isPressed || zahlenfolge.isShown() || this->ui->radioButtonKoords->isChecked()) {
            QImage img = *this->settings.last().image;
            QPainter painter(&img);

            if(mousePress.isPressed && ui->spinBox_zoom->value() > 0) {

                size_t w = this->ui->spinBoxW->value() / ui->spinBox_zoom->value();
                size_t h = this->ui->spinBoxH->value() / ui->spinBox_zoom->value();
                size_t x_left_corner =  mousePress.point.x() - w/2;
                size_t y_left_corner =  mousePress.point.y() - h/2;

                painter.setPen(Qt::black);
                painter.drawRect(QRect(x_left_corner, y_left_corner, w, h));
                painter.setPen(Qt::white);
                painter.drawRect(QRect(x_left_corner - 1, y_left_corner - 1, w + 2, h + 2));
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
                        painter.drawPoint(list.at(i));
                        painter.drawLine(last, list.at(i));
                        if(last != list.at(i))
                            last = list.at(i);
//                        else
//                            break;

                    }


                }
            }


            // optimieren -> nur sichtbares zeug berechnen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(this->ui->radioButtonKoords->isChecked()) {
                painter.setPen(QColor(ui->comboBoxColorKoordSystem->currentText()));
                auto xA = settings.last().xAchse;
                auto yA = settings.last().yAchse;

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
                                         QString::number((double)settings.last().PixelToReal(xA.p1() + QPoint(partsX * i, 0))));
                    }

                    int partsY = yA.dy() / ui->spinBoxBeschriftungenY->value();
                    for(int i = 1; i <= ui->spinBoxBeschriftungenY->value(); i++) {
                        painter.drawLine(yA.p1() + QPoint(-5, partsY * i), yA.p1() + QPoint(5, partsY * i));
                        painter.drawText(yA.p1() + QPoint( -25, partsY * i),
                                         QString::number( - (double)settings.last().PixelToImag(yA.p1() + QPoint(0, partsY * i))));
                    }
                }

            }

//            painter.end();
//            QPainter painter2(ui->labelFraktal);
//            this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));
//            painter2.drawImage(0, 0, img);
//            painter2.end();
            this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));
            i->setImg(img);

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


MainWindow::SETTING::SETTING(long double scale, Point midPoint)
    : scale(scale), x_verschiebung(0), y_verschiebung(0), midPoint(midPoint)
{
    qDebug() << midPoint.x() << " | " << midPoint.y();

}

void MainWindow::SETTING::init(size_t img_w, size_t img_h, size_t maxIterations)
{
    this->img_w = img_w;
    this->img_h = img_h;
    this->maxIterations = maxIterations;

    this->image = new QImage(img_w, img_h, QImage::Format_ARGB32_Premultiplied);
    this->painter = new QPainter(this->image);

    this->x_verschiebung = (ssize_t)midPoint.x() - this->img_w / 2;
    this->y_verschiebung = (ssize_t)midPoint.y() - this->img_h / 2;

    this->iterations = new size_t*[img_w];
    for( size_t i = 0; i < img_w; i++) {
        this->iterations[i] = new size_t[img_h];
        memset(this->iterations[i], 0, sizeof(int) * img_h );
    }

    QPoint startX = QPoint ((-2.5 * scale) - x_verschiebung,
                            ( 0.0 * scale) - y_verschiebung);
    QPoint endeX = QPoint (( 1.5 * scale) - x_verschiebung,
                           ( 0.0 * scale) - y_verschiebung);
    QPoint startY = QPoint ((0.0 * scale) - x_verschiebung,
                           (-1.5 * scale) - y_verschiebung);
    QPoint endeY = QPoint ((0.0 * scale) - x_verschiebung,
                          ( 1.5 * scale) - y_verschiebung);
    this->xAchse = QLine(startX, endeX);
    this->yAchse = QLine(startY, endeY);

}

void MainWindow::SETTING::setIterationCountAt(size_t x, size_t y, size_t iterations)
{
    if(x >= 0 && x < this->img_w && y >= 0 && y < this->img_h) {
        this->iterations[x][y] = iterations;
    }

}

size_t MainWindow::SETTING::getIterationCountAt(QPoint pos)
{
    return this->getIterationCountAt(pos.x(), pos.y());
}

size_t MainWindow::SETTING::getIterationCountAt(size_t x, size_t y)
{
    if(x >= 0 && x < this->img_w && y >= 0 && y < this->img_h)
        return this->iterations[x][y];
    else
        return -1;
}

long double MainWindow::SETTING::PixelToImag(QPoint p)
{
    return (p.y() + this->y_verschiebung) / scale;
}

long double MainWindow::SETTING::PixelToReal(QPoint p)
{
    return (p.x() + this->x_verschiebung) / scale;
}

void MainWindow::on_pushButton_2_clicked()
{
    if(this->settings.length() <= 2 )
        this->ui->pushButton_2->setDisabled(true);
    zustandWechseln("ZURÜCK");
}


void MainWindow::on_pushButton_clicked()
{
    this->ui->pushButton_2->setDisabled(true);
    zustandWechseln("HOME");
}





void MainWindow::on_spinBoxMaxIterations_valueChanged(int arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_comboBox_palette_currentIndexChanged(int index)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_radioButton_normalized_toggled(bool checked)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_radioButton_invert_toggled(bool checked)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_radioButton_toggled(bool checked)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_spinBox_zoom_valueChanged(int arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_spinBoxW_valueChanged(int arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
}


void MainWindow::on_spinBoxH_valueChanged(int arg1)
{
    this->setOperationMode(OP_MODE::APPLY_SETTINGS);
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


void MainWindow::ZahlenFolge::setZahlenfolge(MainWindow::Point c_p, SETTING s)
{
    if(this->isShown())
        this->removeZahlenfolge();

    std::complex<long double> z = std::complex<long double> (0, 0);
    std::complex<long double> c = std::complex<long double>( ((long double)c_p.x() + s.x_verschiebung) / s.scale, - ((long double)c_p.y() + s.y_verschiebung) / s.scale );

    for(long double i = 0; i < s.maxIterations; i++) {
        z = z*z + c;
        if(!std::isfinite(z.real()) || !std::isfinite(z.imag()))
            break;
//        qDebug() << "z:" << (double)z.real() << " + " << (double)z.imag() << "i" << " start: " << (double)c.real() << " + " << (double)c.imag() << "i" ;
        this->zahlenfolge.push_back( QPoint(((z.real() * s.scale)  - s.x_verschiebung) , ( (z.imag() * s.scale) - s.y_verschiebung) ));
    }
//    qDebug() << "this->zahlenfolge:" << this->zahlenfolge ;

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
    auto img = ui->labelFraktal->pixmap();

    img = img.scaled( (double)img.width() * 1.1, (double)img.height() * 1.1, Qt::AspectRatioMode::KeepAspectRatio,  Qt::TransformationMode::SmoothTransformation);
    if( ! img.isNull())
        this->ui->labelFraktal->setPixmap(img);
}
//

void MainWindow::on_pushButton_scale_minus_clicked()
{
    auto img = ui->labelFraktal->pixmap();

    img = img.scaled( (double)img.width() * 0.9, (double)img.height() * 0.9, Qt::AspectRatioMode::KeepAspectRatio,  Qt::TransformationMode::SmoothTransformation);
    if( ! img.isNull())
        this->ui->labelFraktal->setPixmap(img);
}


void MainWindow::Point::setX(long long newX)
{
    X = newX;
}

MainWindow::Point::Point()
{
    X = 0;
    Y = 0;
}

MainWindow::Point::Point(long long X, long long Y)
    : X(X), Y(Y)
{

}

long long MainWindow::Point::y() const
{
    return Y;
}

void MainWindow::Point::setY(long long newY)
{
    Y = newY;
}

long long MainWindow::Point::x() const
{
    return X;
}


MainWindow::MousePress::MousePress()
{

}

void MainWindow::MousePress::updateKoords(Point p, size_t x_left_corner, size_t add_y)
{
    point = p;
    koord = Point(p.x() + x_left_corner, p.y() + add_y);
}

void MainWindow::MousePress::setPressed()
{
    isPressed = true;
}
