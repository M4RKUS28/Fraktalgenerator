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


void MainWindow::zustandWechseln(QString aktion, QString, int, QMouseEvent *m_event)
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
            if(this->zoomOrReload == "RELOAD") {
                //Aktualiseren
                this->startRefresh(settings.last());

            } else {
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
            if(this->mousePress.isPressed) {
                this->mousePress.setPress( this->ui->labelFraktal->mapFrom(this, m_event->pos()), settings.last().x_verschiebung, settings.last().y_verschiebung  );
                this->zoomOrReload = ("ZOOM");
                ui->pushButtonStart->setText("Vergrößern");
                this->update();
            }

            auto pos = this->ui->labelFraktal->mapFrom(this, m_event->pos());
            qDebug() << pos;
            if(pos.x() >= 0 && pos.x() < settings.last().img_w && pos.y() >= 0 && pos.y() < settings.last().img_h) {
                ui->label_iterations->setText(QString::number(settings.last().iterations[pos.x()][pos.y()]));

                ui->re->setText(QString::number((double(this->mousePress.koord.x() / settings.last().scale))));
                ui->img->setText(QString::number((double(this->mousePress.koord.y() / settings.last().scale)))  + "i");
            } else {
                ui->label_iterations->setText("-");
                ui->re->setText("-");
                ui->img->setText("-");
            }

        } else if(aktion == "mousePressEvent"  && m_event) {

            auto pos = this->ui->labelFraktal->mapFrom(this, m_event->pos());
            if(m_event->button() == Qt::LeftButton && pos.x() >= 0 && pos.x() < settings.last().img_w && pos.y() >= 0 && pos.y() < settings.last().img_h) {
                this->mousePress.setPress(pos , settings.last().x_verschiebung, settings.last().y_verschiebung );
                this->zoomOrReload = ("ZOOM");
                ui->pushButtonStart->setText("Vergrößern");

                ui->label_iterations->setText(QString::number(settings.last().iterations[pos.x()][pos.y()]));
                ui->re->setText(QString::number((double(this->mousePress.koord.x() / settings.last().scale))));
                ui->img->setText(QString::number((double(this->mousePress.koord.y() / settings.last().scale)))  + "i");

                this->update();
            } else {
                this->zoomOrReload = ("RELOAD");
                ui->pushButtonStart->setText("Neu Laden");
                this->mousePress.isPressed = false;

                ui->re->setText("-");
                ui->img->setText("-");
                ui->label_iterations->setText("-");

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

    // Zoom midpoint --> new scaling!!!!
    int midx = (this->mousePress.point.x() + lastSet.x_verschiebung)* (long double)(ui->spinBox_zoom->value());
    int midy= (this->mousePress.point.y() + lastSet.y_verschiebung)* (long double)(ui->spinBox_zoom->value());

    SETTING s(lastSet.scale * (long double)(ui->spinBox_zoom->value()), QPoint(midx, midy));
    s.init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value());
    return s;
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
            v = (iters == 0) ? 255.0 /*Mandelbrot-Set-Alpha*/ : ((double)((int)(n) % 255) / 255.0);
            v = ui->radioButton_invert->isChecked() ? 255.0 - v : v;
            mpainter->setPen(QColor::fromRgbF(1, v, v, 1 ));
            break;
        default:
            alpha = (iters == 0) ? 1.0 /*Mandelbrot-Set-Alpha*/ : 0.0; /*Outside*/
            mpainter->setPen(QColor::fromRgbF(0, 0, 0, ui->radioButton_invert->isChecked() ? 1.0 - alpha : alpha ));
            break;
        }
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


    //calc work for each thread
    int tc = std::max(std::min(ui->spinBox_threads->value(), set.img_w), 1);
    int parts = set.img_w / tc;

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
    int x_left_corner = set.x_verschiebung;
    int y_left_corner = set.y_verschiebung;

    // wenn nicht nur neuladen --> add new settings to list
    if(zoom)
        settings.push_back(set);


    //start workers
    for( int tn = 0; tn < tc; tn++) {
        WorkerThread * wt = new WorkerThread(this);
        this->tworkers.append( wt );

        wt->startCalc(x_left_corner + parts * tn,
                      x_left_corner + parts * (tn + 1) + ( (tn + 1 == tc ) ? (set.img_w - parts * (tn + 1)) : 0), // wenn breite mit threadzahl nicht aufgeht -> letzter thread noch etwas breiter
                      y_left_corner,
                      y_left_corner + set.img_h,
                      ui->spinBoxIterations->value(),
                      ui->doubleSpinBoxEscapeR->value(),
                      (1.0 / set.scale),
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
    this->zoomOrReload = "RELOAD";
    ui->progressBar->setEnabled(false);
    ui->pushButtonSaveImg->setEnabled(true);
    ui->frameButtons->setEnabled(true);
    ui->lineEditScaleAbs->setText("1:" + QString::number((double)settings.last().scale));
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->bmRe->setText(QString::number( (double)(((long double) this->settings.last().midPoint.x()) / this->settings.last().scale) ));
    ui->bmIm->setText(QString::number( (double)(((long double) this->settings.last().midPoint.y()) / this->settings.last().scale) ) + "i");
}

void MainWindow::loadInitImg()
{
    //später das zuletzt verwendete
    SETTING newImage(150, QPoint(-80, 0));
    newImage.init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value());
    newImage.painter->fillRect(newImage.image->rect(), QColor(ui->comboBox_background_color->currentText()));

    this->ui->labelFraktal->setPixmap(QPixmap::fromImage(*newImage.image));
    this->ui->labelFraktal->setFixedSize( ui->spinBoxW->value(), ui->spinBoxH->value());

    this->startRefresh(newImage, true);
}

void MainWindow::updateImage()
{
    if(settings.empty())
        return;
    auto img = QPixmap::fromImage(*settings.last().image);
    if( ! img.isNull())
        this->ui->labelFraktal->setPixmap(img);
}

bool MainWindow::checkForFinished()
{
    if(this->ui->progressBar->value() == this->ui->progressBar->maximum() ) {
        for ( int i = 0; i <  this->tworkers.length(); i++ ) {
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
    for ( int i = 0; i <  this->tworkers.length(); i++ ) {
        if (this->tworkers.at(i)->isRunning()) {
            this->tworkers.at(i)->quit();
            if(!this->tworkers.at(i)->wait(100)) {
                this->tworkers.at(i)->terminate();
            }
        }
    }

    for ( int i = 0; i <  this->tworkers.length(); i++ ) {
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
    for( int l = 0; l < list->size(); l++) {
        this->setColor(settings.last().painter, list->at(l).i, list->at(l).z_n);
        int x = list->at(l).c_x - settings.last().x_verschiebung,
            y = list->at(l).c_y -  settings.last().y_verschiebung;
        settings.last().painter->drawPoint(x, y);
        if(x >= 0 && x < this->settings.last().img_w && y >= 0 && y < this->settings.last().img_h)
            settings.last().iterations[x][y] = list->at(l).i;

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

void MainWindow::paintEvent(QPaintEvent *event)
{
    switch (state) {
    case MainWindow::INIT:
        break;
    case MainWindow::STOPED:

        if(mousePress.isPressed) {

            QImage img = *this->settings.last().image;
            int w = this->ui->spinBoxW->value() / (double)(ui->spinBox_zoom->value());
            int h = this->ui->spinBoxH->value() / (double)(ui->spinBox_zoom->value());
            int x_left_corner =  mousePress.point.x() - w/2;
            int y_left_corner =  mousePress.point.y() - h/2;

            QPainter painter(&img);
            painter.setPen(Qt::black);
            painter.drawRect(QRect(x_left_corner, y_left_corner, w, h));
            painter.setPen(Qt::white);
            painter.drawRect(QRect(x_left_corner - 1, y_left_corner - 1, w + 2, h + 2));
            painter.end();
            this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));
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


MainWindow::SETTING::SETTING(long double scale, QPoint midPoint)
    : scale(scale), x_verschiebung(0), y_verschiebung(0), midPoint(midPoint)
{

}

void MainWindow::SETTING::init(int img_w, int img_h)
{
    this->img_w = img_w;
    this->img_h = img_h;
    this->image = new QImage(img_w, img_h, QImage::Format_ARGB32_Premultiplied);
    this->painter = new QPainter(this->image);

    this->x_verschiebung = midPoint.x() - this->img_w / 2;
    this->y_verschiebung = midPoint.y() - this->img_h / 2;

    this->iterations = new int*[img_w];
    for( int i = 0; i < img_w; i++) {
        this->iterations[i] = new int[img_h];
        memset(this->iterations[i], 0, sizeof(int) * img_h );
    }
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



void MainWindow::on_actionSpeichern_unter_triggered()
{
    ui->pushButtonSaveImg->click();
}

#include <QClipboard>

void MainWindow::on_actionBild_in_Zwischenablage_kopieren_triggered()
{
    QApplication::clipboard()->setImage(*this->settings.last().image);
    this->ui->statusbar->showMessage("Bild in Zwischenablage kopiert.", 2000);
}

