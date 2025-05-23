#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QTime>

#include <QScreen>
#include <QApplication>
#include <QSysInfo>
#include <QStyledItemDelegate>
#include <QSettings>
#include <QTimer>
#include "dialogcreatevideo.h"
#include "QAviWriter.h"
#include <QSplitter>

#include "math.h"

//Qt 6+
#include <QStyleHints>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    defaultStyle( QApplication::style()->objectName() ),
    currentImg(new ImageSetting(0, START_SCALE * this->devicePixelRatio() * ( QGuiApplication::primaryScreen()->geometry().height() == 1440 ? 2 : 1 ) , START_POS_X , START_POS_Y )),
    lastImgStructID(0),
    editedSettings(false),
    noUpdateGui(false),
    isBackOrVor(false),
    ignoreXPosEdited(false),
    ignoreYPosEdited(false),
    dont_update_save_check_buttons(false),
    currentBackgroundColor(QColor("white")),
    fullScreenView(new ImageView),
    codeExecuted(false),
    useOldPosForImgSerie(false),
    ui(new Ui::MainWindow)
{
    qInfo() << "Init programm...";

    //SETUP DEFAULT COLORS/PALETTE:
    //0
    default_colors.push_back(QList<QColor>());
    //1
    default_colors.push_back(QList<QColor>());

    //2
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(255,255,255));

    //3
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(255,255,255));

    //4
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(255, 0, 0));
    default_colors.back().push_back(QColor::fromRgb(255, 255, 0));
    default_colors.back().push_back(QColor::fromRgb(0,255,0));
    default_colors.back().push_back(QColor::fromRgb(0, 255, 255));
    default_colors.back().push_back(QColor::fromRgb(0, 0, 255));
    default_colors.back().push_back(QColor::fromRgb(255,0,255));

    //5
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(37,27,255));
    default_colors.back().push_back(QColor::fromRgb(235, 156, 18));
    default_colors.back().push_back(QColor::fromRgb(238, 31, 235));

    //6
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(0, 255, 255));
    default_colors.back().push_back(QColor::fromRgb(235, 180, 15));

    //7
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(20, 200, 0));
    default_colors.back().push_back(QColor::fromRgb(0, 0, 0));
    default_colors.back().push_back(QColor::fromRgb(25,165,255));
    default_colors.back().push_back(QColor::fromRgb(0, 0, 0));
    default_colors.back().push_back(QColor::fromRgb(255, 205, 0));
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(255,0,0));

    //8
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(0, 10, 255));
    default_colors.back().push_back(QColor::fromRgb(255, 120, 0));
    default_colors.back().push_back(QColor::fromRgb(255,225,5));
    default_colors.back().push_back(QColor::fromRgb(255, 255, 0));
    default_colors.back().push_back(QColor::fromRgb(15, 205, 0));
    default_colors.back().push_back(QColor::fromRgb(10,65,255));
    default_colors.back().push_back(QColor::fromRgb(255,15,215));

    //9
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(0,0,0));
    default_colors.back().push_back(QColor::fromRgb(255, 180, 0));

    //10
    default_colors.push_back(QList<QColor>());

    //11 HSV
    default_colors.push_back(QList<QColor>());
    default_colors.back().push_back(QColor::fromRgb(255, 0, 0));
    default_colors.back().push_back(QColor::fromRgb(255, 255, 0));
    default_colors.back().push_back(QColor::fromRgb(0,255,0));
    default_colors.back().push_back(QColor::fromRgb(0, 255, 255));
    default_colors.back().push_back(QColor::fromRgb(0, 0, 255));
    default_colors.back().push_back(QColor::fromRgb(255,0,255));

    //12 HSV
    default_colors.push_back(default_colors.back());

    //Histogramm
    default_colors.push_back(QList<QColor>());
    //|
    default_colors.push_back(default_colors.at(default_colors.length() -2));



    this->fullScreenView->hide();
    this->fullScreenView->setAttribute( Qt::WA_QuitOnClose, false );

    keyPressed[Qt::LeftButton] = false;
    keyPressed[Qt::RightButton] = false;

    ///setup ui
    ui->setupUi(this);
#ifdef SINGLE_THREADTED_WEB_ASSEMBLY
//    this->ui->spinBox_threads->setMaximum(1);
#endif

    ui->listWidgetHistory->setContextMenuPolicy(Qt::CustomContextMenu);

    ///-------------------------

    ui->comboBox_palette->setCurrentIndex(2);
    on_comboBox_palette_currentIndexChanged(2);


    //Qt 6+
    ui->progressBar->setTextVisible(ui->comboBox_theme->currentIndex() != 0);
    if(QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Light)
        currentBackgroundColor = "white";
    else
        currentBackgroundColor = QColor(60, 60, 60);



    //Setup comobox -> fix error on mac
    ui->comboBox_palette->setItemDelegate(new QStyledItemDelegate(ui->comboBox_palette));    
    setupIconsInCombobox(80);

    ui->tabWidget->setCurrentIndex(0);
    //die einstellungen sind unnütz:
    ui->comboBox_precession->setHidden(true);

    //Hide extra pushButtonFraktalColor option button
    ui->pushButtonFraktalColor->setHidden(true);


    if( QSysInfo::productType() == "macos" /*Qt 6*/ || QSysInfo::productType() == "macos"  /*Qt 5*/ ) {
        // REMOVE FULL SCREEN MODE IN MAC, because not working at the moment

        ui->actionVollbildfenster_anzeigen->setDisabled(true);

        ui->spinBoxHauptScreen->setVisible(false);
        ui->label_34->setVisible(false);


        //HIER DIE SCHRIFTGRÖßEN ALLER WIDGETS SETZTEN && LAYOUT AUSRICHTUNG && AUTO INCREASEMENT
        //-> Wechselt in Mac zu standart Mac schrift, die kleiner ist!!!!
        /*
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         *
         * */

//        QFont f = this->font();
//        f.setPixelSize(13);
//        this->setFont(f);




    }

    ui->comboBox_palette->setCurrentIndex(6);

    //-------------SETUP COLOR BUTTONS.....:------------------------

    //clear save button array
    for(int i = 0; i < 8; i++) {
        buttonColors_save[i] = QColor::fromRgb(255, 255, 255);
        isChecked_save[i] = false;
    }  isChecked_save[0] = true;

    //clear fraktal eigen-color
    fraktalColor = QColor::fromRgb(255, 255, 255);
    ui->pushButtonFraktalColor->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + fraktalColor.name() );

    //load color from qsetting:
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    if(settingOwnColor.contains("OWN_FRAKTAL_COLOR"))
        fraktalColor = settingOwnColor.value("OWN_FRAKTAL_COLOR").toString();
    else
        fraktalColor = QColor::fromRgb(255, 255, 255);
    ui->pushButtonFraktalColor->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + fraktalColor.name() );

    for(int i = 0; i < 8; i++) {
        if(settingOwnColor.contains("OWN_RGB_VERLAUF_OWN_COLOR_" + QString::number(i+1)))
            buttonColors_save[i] = settingOwnColor.value("OWN_RGB_VERLAUF_OWN_COLOR_" + QString::number(i+1)).toString();
        if(settingOwnColor.contains("OWN_RGB_VERLAUF_OWN_CHECKED_" + QString::number(i+1)))
            isChecked_save[i] = settingOwnColor.value("OWN_RGB_VERLAUF_OWN_CHECKED_" + QString::number(i+1)).toBool();


    }

    //update combox icons
    updateIconInOwnColor(80);


    ///------------------------------------
    //save start size
    int width = (QApplication::screens().at(0)->geometry().width() * QApplication::screens().at(0)->devicePixelRatio()),
        height = (QApplication::screens().at(0)->geometry().height() * QApplication::screens().at(0)->devicePixelRatio());

    //setup currentImage
    currentImg->init(width, height, ui->spinBoxMaxIterations->value(), true);
    currentImg->painter->fillRect(currentImg->image->rect(), currentBackgroundColor);

    // connect signals
    connect(ui->imageView, SIGNAL(mouseMove(QPoint)), this, SLOT(mouse_move_in_img(QPoint)));
    connect(ui->imageView, SIGNAL(mousePress(QMouseEvent*, QPoint )), this, SLOT(mouse_press_in_img(QMouseEvent*, QPoint)));
    connect(ui->imageView, SIGNAL(mouseRelease(QMouseEvent*, QPoint)), this, SLOT(mouse_release_in_img(QMouseEvent*, QPoint)));
    connect(ui->imageView, SIGNAL(mouseDoubleClick(QMouseEvent*, QPoint)), this, SLOT(mouse_double_click_in_img(QMouseEvent*, QPoint)));
    connect(ui->imageView, SIGNAL(pinchEvent(bool,QPoint,QPoint)), this, SLOT(pinchRecieved(bool,QPoint,QPoint)));
    this->ui->imageView->setAllowZooming(true);


    connect(fullScreenView, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(key_press_in_img(QKeyEvent *)));
    connect(fullScreenView, SIGNAL(mouseMove(QPoint)), this, SLOT(mouse_move_in_img(QPoint)));
    connect(fullScreenView, SIGNAL(mousePress(QMouseEvent*, QPoint)), this, SLOT(mouse_press_in_img(QMouseEvent*, QPoint)));
    connect(fullScreenView, SIGNAL(mouseRelease(QMouseEvent*, QPoint)), this, SLOT(mouse_release_in_img(QMouseEvent*, QPoint)));
    connect(fullScreenView, SIGNAL(mouseDoubleClick(QMouseEvent*, QPoint)), this, SLOT(mouse_double_click_in_img(QMouseEvent*, QPoint)));
    connect(fullScreenView, SIGNAL(pinchEvent(bool,QPoint,QPoint)), this, SLOT(pinchRecieved(bool,QPoint,QPoint)));

    // init rect:
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    zoomRect.updateRectPos(QPoint(currentImg->mapGausReToImgPos(currentImg->gaus_mid_re), currentImg->mapGausImToImgPos(currentImg->gaus_mid_im)),
                           QPointF(currentImg->gaus_mid_re, currentImg->gaus_mid_im) );

    //setup ui values
    this->ui->spinBoxW->setValue( width );
    this->ui->spinBoxH->setValue( height );

    //start timer
    timerID = this->startTimer(30);


    ///LOAD DEFAULTS...........................................................................
    // standart theme
    if(settingOwnColor.contains("THEME"))
        ui->comboBox_theme->setCurrentIndex(settingOwnColor.value("THEME").toInt());
    else
        ui->comboBox_theme->setCurrentIndex(5);

    // anzahl an cpu cors als standart
    if(settingOwnColor.contains("THREAD_COUNT"))
        this->ui->spinBox_threads->setValue(settingOwnColor.value("THREAD_COUNT").toInt());
    else
        this->ui->spinBox_threads->setValue(QThread::idealThreadCount() * 2 /*SPEED BOOST ????*/);

    if(settingOwnColor.contains("THREAD_CREATE_TIME_DELAY"))
        this->ui->spinBoxStartVerzoegerung->setValue(settingOwnColor.value("THREAD_CREATE_TIME_DELAY").toInt());
    else
        this->ui->spinBoxStartVerzoegerung->setValue(10);

    if(settingOwnColor.contains("ESCAPE_RADIUS"))
        this->ui->doubleSpinBoxEscapeR->setValue(settingOwnColor.value("ESCAPE_RADIUS").toDouble());
    else
        this->ui->doubleSpinBoxEscapeR->setValue(8.0);

    //UPdater.....
    updater = new Updater(/*QApplication::applicationDirPath() +*/ "C:/Program Files/Fraktalgenerator/bin/../FraktalgeneratorMaintenanceTool.exe", "Markus@W-Sem_2022", "Fraktalgenerator"); // auto search for updates
    ui->actionNach_Updates_suchen->setChecked(updater->getAutoSearchForUpdateStatus());

}

void MainWindow::showEvent(QShowEvent *)
{
    if (!codeExecuted) {
        codeExecuted = true;
        QTimer::singleShot(0, this, [this]() {
            // Your code to be executed after QMainWindow is fully constructed
            qDebug() << "QMainWindow is fully constructed.";
            //Start leoading init image
            updateImage();
            this->startRefresh(currentImg, true);

            QList<int> sizes;
            sizes.push_back(ui->splitter->width() - init_width_sidebar);
            sizes.push_back(init_width_sidebar);
            ui->splitter->setSizes(sizes);

        });
    }
}


void MainWindow::setupIconsInCombobox(int width)
{
    for(int i = 0; i < default_colors.length(); i++) {
        int color_count = default_colors.at(i).length();
        if(color_count <= 0)
            continue;

        QImage img = QImage(width, 16, QImage::Format_ARGB32_Premultiplied);
        QPainter p = QPainter(&img);

        for(int x = 0; x < color_count; x++ )
            p.fillRect(QRect((width / color_count) * x, 0, (x == (color_count-1)) ? (width - (width / color_count) * x) : (width / color_count) , 16), default_colors.at(i).at(x));

        p.setPen(QPen(Qt::black, 1));
        p.drawRect(0, 0, width-1, 16-1);

        ui->comboBox_palette->setItemIcon(i, QIcon(QPixmap::fromImage(img)));
    }

    for( auto &combobox : {ui->comboBoxMandelColor,
                           ui->comboBoxColorKoordSystem,
                           ui->comboBox_color_zahlenfolge}) {
        for(int i = 0; i < combobox->count(); i++) {
            if(!QColor::isValidColorName(combobox->itemText(i)))
                continue;

            QImage img = QImage(32, 16, QImage::Format_ARGB32_Premultiplied);
            QPainter p = QPainter(&img);
            p.fillRect(0, 0, 32, 16, QColor(combobox->itemText(i)));
            p.setPen(QPen(Qt::black, 1));
            p.drawRect(0, 0, 32-1, 16-1);
            combobox->setItemIcon(i, QIcon(QPixmap::fromImage(img)));
        }
    }

    ui->comboBox_Fraktal->setItemIcon(0, QIcon(QPixmap::fromImage(QImage(":/icons/icons/mandelbrot_menge_256x128.png"))));
    ui->comboBox_Fraktal->setItemIcon(1, QIcon(QPixmap::fromImage(QImage(":/icons/icons/julia_menge_256x128 - Julia-Menge.png"))));
}

void MainWindow::updateIconInOwnColor(int width)
{
    QImage img = QImage(width, 16, QImage::Format_ARGB32_Premultiplied);
    QPainter p = QPainter(&img);
    QList<QColor> colors;

    int color_count = 0;
    for(int i = 0; i < 8; i++) {
        if(isChecked_save[i]) {
            color_count ++;
            colors.append(buttonColors_save[i]);
        }
    }
//    if(ui->radioButtonF1->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[0]);
//    }
//    if(ui->radioButtonF2->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[1]);
//    }
//    if(ui->radioButtonF3->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[2]);
//    }
//    if(ui->radioButtonF4->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[3]);
//    }
//    if(ui->radioButtonF5->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[4]);
//    }
//    if(ui->radioButtonF6->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[5]);
//    }
//    if(ui->radioButtonF7->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[6]);
//    }
//    if(ui->radioButtonF8->isChecked()) {
//        color_count ++;
//        colors.append(buttonColors_save[7]);
//    }

    p.fillRect(0, 0, width, 16, Qt::white);

    for(int x = 0; x < color_count; x++ )
        p.fillRect(QRect((width / color_count) * x, 0, (x == (color_count-1)) ? (width - (width / color_count) * x) : (width / color_count) , 16), colors[x]);

    p.setPen(QPen(Qt::black, 1));
    p.drawRect(0, 0, width-1, 16-1);

    ui->comboBox_palette->setItemIcon(1, QIcon(QPixmap::fromImage(img)));
}

MainWindow::~MainWindow()
{
    //Stoppe timer
    this->killTimer(timerID);

    // Stoppe und lösche Threads...
    stopThreads();

    delete updater;
    updater = nullptr;

    delete fullScreenView;
    fullScreenView = nullptr;

    //Lösche die Bilderliste
    for( auto e : settingsList) {
        e->cleanUP();
        delete e;
    }
    settingsList.clear();

    delete ui;
    ui = nullptr;

    qDebug() << "~MainWindow";
    qInfo() << "Exit programm(0)";
}



void MainWindow::updateMidPos(bool clear)
{

    this->ignoreXPosEdited = true;
    this->ignoreYPosEdited = true;
    if(clear) {

        ui->re->setValue(0);
        ui->im->setValue(0);
    } else {
        ui->re->setValue(this->zoomRect.getMausPosInGauss().x());
        ui->im->setValue(- this->zoomRect.getMausPosInGauss().y());
        ui->statusbar->showMessage("Re(" + QString::number(this->zoomRect.getMausPosInGauss().x(), 'g', 15)
                                   + ") Im(" + QString::number( - this->zoomRect.getMausPosInGauss().y(), 'g', 15) + ")", 2000);

    }

}

void MainWindow::updateButtonColors()
{
    ui->pushButton_color1->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[0].name() );
    ui->pushButton_color2->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[1].name() );
    ui->pushButton_color3->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[2].name() );
    ui->pushButton_color4->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[3].name() );
    ui->pushButton_color5->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[4].name() );
    ui->pushButton_color6->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[5].name() );
    ui->pushButton_color7->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[6].name() );
    ui->pushButton_color8->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[7].name() );
}


void MainWindow::timerEvent(QTimerEvent *)
{
    zustandWechseln("TIMER");
}


void MainWindow::zustandWechseln(QString aktion, QString param, QPoint m_pos, QMouseEvent *m_event)
{
    if(aktion != "TIMER" && aktion != "mouseMoveEvent")
        qDebug() << "zustandWechseln "<< aktion;

    switch (this->state) {
    case MainWindow::STOPED:

        if(aktion == "TIMER") {

        } else if(aktion == "BUTTON_REFRESH_AND_STOP") {

            if(this->op_mode == OP_MODE::REFRESH ) {
                //Aktualiseren
                this->startRefresh(currentImg);

            // sonst speichere in history
            } else {
                //Zoomen
                ImageSetting *newSetting = getNewScaledSetting(currentImg);
                this->startRefresh(newSetting, true);

            }

        } else if( aktion == "DOPPELKLICK_IN_BILD" && m_event) {
            //update rect if no mousemove or mouse press was done --> fix doppelclick no zoom just refresh error
            zustandWechseln("mousePressEvent", "", m_pos, m_event);
            zustandWechseln("mouseReleaseEvent", "", m_pos, m_event);

            auto pos = m_pos; //m_event->pos() * ui->imageView->getHdpi_multiplicator();

            if(m_event->button() == Qt::LeftButton && pos.x() >= 0 && pos.x() < currentImg->img_w && pos.y() >= 0 && pos.y() < currentImg->img_h) {
                ImageSetting *newSetting = getNewScaledSetting(currentImg);
                this->startRefresh(newSetting, true);

            }
        } else if(aktion == "mouseReleaseEvent" && m_event) {
            keyPressed[m_event->button()] = false;
            if(m_event->button() == Qt::LeftButton) {
                // no update in paintevent!! --> no move!!
                this->zoomRect.setMousePressState(false);
                this->update();
            }

        } else if(aktion == "mouseMoveEvent") {
            auto qpos = m_pos;
            auto pos = Point(qpos.x(), qpos.y());

            if(pos.x() >= 0 && pos.x() < currentImg->img_w && pos.y() >= 0 && pos.y() < currentImg->img_h) {

                if(keyPressed[Qt::LeftButton] || keyPressed[Qt::RightButton]) {

                    this->zoomRect.updateRectPos( pos.rountToQPoint(), QPointF( currentImg->mapImgPosReToGaus(pos.x()), currentImg->mapImgPosImToGaus(pos.y()) ) );

                    if(this->zoomRect.rightMouseIsPressed()) {
                        this->setOperationMode();
                        this->updateImage();
                    }
                    ui->label_iterations->setText(QString::number(currentImg->getIterationCountAt(qpos)));

                    updateMidPos();


                    if( zahlenfolge.isShown() ) {
                        this->zahlenfolge.setZahlenfolge(pos, currentImg);
                        this->updateImage();
                    }
                }



            } else {
                ui->label_iterations->setText("-");
//                ui->re->setValue(0);
//                ui->im->setValue(0);
            }

        } else if(aktion == "mousePressEvent"  && m_event) {
            keyPressed[m_event->button()] = true;

//            auto qpos = this->ui->imageView->mapFrom(this, m_event->pos()) * ui->imageView->getHdpi_multiplicator();
            auto qpos = m_pos;// m_event->pos() * ui->imageView->getHdpi_multiplicator();
            auto pos = Point(qpos.x(), qpos.y());
            qDebug() << qpos;

            if(pos.x() >= 0 && pos.x() < currentImg->img_w && pos.y() >= 0 && pos.y() < currentImg->img_h && param != "NOT_IN_IMG"
                    /*&& m_event->pos().x() < ui->scrollArea->width() + ui->scrollArea->pos().x() + 10
                    && m_event->pos().y() < ui->scrollArea->height() + ui->scrollArea->pos().y() + 10*/) {
                if(m_event->button() == Qt::LeftButton) {
                    zoomRect.setMousePressState(true);
                    zoomRect.show();

                    this->zoomRect.updateRectPos(pos.rountToQPoint(), QPointF( currentImg->mapImgPosReToGaus(pos.x()), currentImg->mapImgPosImToGaus(pos.y()) ));
                    this->setOperationMode();

                    ui->label_iterations->setText(QString::number(currentImg->getIterationCountAt(qpos)));

                    updateMidPos();

                    if(zahlenfolge.isShown())
                        this->zahlenfolge.removeZahlenfolge();
                } else  if(m_event->button() == Qt::RightButton) {
                    this->zahlenfolge.setZahlenfolge(pos, currentImg);
                    this->zoomRect.hide();
                    this->setOperationMode();
                }

                this->updateImage();
            } else {
                if(m_event->button() != Qt::LeftButton) {
                    zoomRect.setMousePressState(false);
                    zoomRect.hide();
                    this->setOperationMode();
    //                updateMidPos(true);
                }

                ui->label_iterations->setText("-");
                this->zahlenfolge.removeZahlenfolge();
                this->setOperationMode();

                updateImage();
            }


        } else if(aktion == "ZURÜCK") {
            int pos = - 1;
            for(int i = 0; i < settingsList.length(); i++)
                if(settingsList.at(i)->id == currentImg->id)
                    pos = i - 1;

            if(pos >= 0 && pos < settingsList.length()) {
                //HIER AKTUELLES AUWÄHLEN!!!!
                //ui->listWidgetHistory->setCurrentRow(pos);

                this->currentImg = settingsList.at(pos);

                if(ui->actionEinstellungen_bei_Verlaufwechsel_behalten->isChecked()) {
                    this->noUpdateGui = true;
                    this->isBackOrVor = true;
                }

                if(ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked())
                    this->startRefresh(currentImg);
                else {
                    this->endRefresh(false);
                    updateImage();
                }

                for(int i = 0; i < ui->listWidgetHistory->count(); i++)
                    if(ui->listWidgetHistory->item(i)->data(187).toInt() == currentImg->id)
                        ui->listWidgetHistory->setCurrentRow(i);
                ui->pushButton_vor->setDisabled(false);
            }

            if(pos == 0)
                ui->pushButton_back->setEnabled(false);

        } else if(aktion == "VOR") {
            int pos = - 1;
            for(int i = 0; i < settingsList.length(); i++)
                if(settingsList.at(i)->id == currentImg->id)
                    pos = i + 1;

            if(pos >= 0 && pos < settingsList.length()) {
                this->currentImg = settingsList.at(pos);

                if(ui->actionEinstellungen_bei_Verlaufwechsel_behalten->isChecked()) {
                    this->noUpdateGui = true;
                    this->isBackOrVor = true;
                }

                if(ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked())
                    this->startRefresh(currentImg);
                else {
                    this->endRefresh(false);
                    updateImage();
                }

                for(int i = 0; i < ui->listWidgetHistory->count(); i++)
                    if(ui->listWidgetHistory->item(i)->data(187).toInt() == currentImg->id)
                        ui->listWidgetHistory->setCurrentRow(i);
                ui->pushButton_back->setDisabled(false);
            }

            ui->pushButton_vor->setDisabled(false);
            if(pos == settingsList.length()-1)
                ui->pushButton_vor->setDisabled(true);


        } else if(aktion == "HOME") {

            // Reset
            if(this->currentImg->id == settingsList.at(0)->id) {
                this->noUpdateGui = false;
                //reset

                if(ui->actionEinstellungen_bei_Verlaufwechsel_behalten->isChecked() && ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked()) {
                    this->isBackOrVor = true;
                }

            } else {
                this->noUpdateGui = true;

                if(ui->actionEinstellungen_bei_Verlaufwechsel_behalten->isChecked()) {
                    this->isBackOrVor = true;
                }
            }


            this->currentImg = settingsList.at(0);
            if(ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked())
                this->startRefresh(currentImg);
            else {
                this->endRefresh(false);
                updateImage();
            }

            for(int i = 0; i < ui->listWidgetHistory->count(); i++)
                if(ui->listWidgetHistory->item(i)->data(187).toInt() == currentImg->id)
                    ui->listWidgetHistory->setCurrentRow(i);
            ui->pushButton_back->setDisabled(true);
            if(ui->listWidgetHistory->count() > 1)
                ui->pushButton_vor->setDisabled(false);
            else
                ui->pushButton_vor->setDisabled(true);

//            ui->pushButtonHome->setText("Reset");

        }
        break;
    case MainWindow::RUNNING:
        if(aktion == "TIMER") {
            if(imgUpdateNeeded)
                updateImage();
            if(checkForFinished()) {
                qInfo() << "    -> Finished!";

                updateImage();
                endRefresh(op_mode != OP_MODE::REFRESH);
            }

        } else if(aktion == "BUTTON_REFRESH_AND_STOP") {

            qInfo() << "    -> Abort!";

            // stop img serie
            imgSerie.imgCount = 0;
            this->stopThreads();
            this->endRefresh(op_mode != OP_MODE::REFRESH);
        }

        break;

    }
}

ImageSetting *MainWindow::getNewScaledSetting(ImageSetting *last_img)
{
    // Zoom midposize_t --> new scaling!!!!
    double midx =0 ;
    double midy =0 ;
    double nscale = 0;

    // 4 möglichkeiten: wechsel von mandelbrot zu julia --> calc julia an stelle mauspos der mandelbrot menge
    //  wechsel von julia zu mandelbrot --> starteinstellungen der mandelbrot menge
    // zoom in fraktal ( mit speicherung der alten julia einstellungen ) oder reload --> kein zoom

    // 1)
    if(last_img->isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 1 /*julia*/ ) {
        //create new julia-funktion on midbpoint of last mandelbrot-funktion
        ImageSetting *j = new ImageSetting( (this->lastImgStructID = this->lastImgStructID + 1), START_SCALE  * this->devicePixelRatio() , START_POS_X_JUL, START_POS_Y_JUL );
        j->init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBoxMaxIterations->value(), false /*julia menge!*/ );
        j->juliaStart_real = ui->re->value();
        j->juliaStart_img = -ui->im->value();

//        j.isStart = true; // save start for hom button
        return j;

        // 2)
    } else if( ! last_img->isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 0 /*mandelbrot*/) {
        ImageSetting *m = new ImageSetting((this->lastImgStructID = this->lastImgStructID + 1), START_SCALE, START_POS_X_JUL, START_POS_Y_JUL );
        m->init(ui->spinBoxW->value(), ui->spinBoxH->value(), ui->spinBoxMaxIterations->value(), true);
        return m;

    } else {

        //es wird davon ausgegangen, dass diese Funktion dann aufgerufen wird, wenn mousePress gesetzt ist! Wenn nicht, dann sollte lastSetGeladen werden.
        // entweder zoomfeld gesetzt und davon mitte, egal ob doppelklick oder button, oder reines neu laden button!!
        double relativeZoom = ui->spinBox_zoom->value();
        if(ui->spinBox_zoom->value() == 0.0)
            ui->spinBox_zoom->setValue( (relativeZoom = 1.0) );

        //3)
       if(this->zoomRect.isShown() /*&& (last_img.scale * relativeZoom) < (std::pow(10, 16))*/) {

           // bei bilder serie verwende alte koordinaten verwenden, aber erst ab 2ten mal!
           if(imgSerie.imgCount > 0 && useOldPosForImgSerie) {
               if(relativeZoom > 0) {
                   nscale = last_img->scale() * relativeZoom;
                   midx    = last_img->gaus_mid_re;
                   midy    = last_img->gaus_mid_im;
               } else {
                   nscale  = -last_img->scale() / relativeZoom;
                   midx    = last_img->gaus_mid_re;
                   midy    = last_img->gaus_mid_im;
               }

           }
           // verwende neue Maus Posaition
           else {
               if(relativeZoom > 0) {
                   nscale = last_img->scale() * relativeZoom;
                   midx   =  this->zoomRect.getMausPosInGauss().x();
                   midy   =  this->zoomRect.getMausPosInGauss().y();
               } else {
                   nscale  = -last_img->scale() / relativeZoom;
                   midx    = this->zoomRect.getMausPosInGauss().x();
                   midy    = this->zoomRect.getMausPosInGauss().y();
               }

           }

            //4)
        } else {
//           if((last_img.scale * relativeZoom) > (std::pow(10, 16)))
//               QMessageBox::warning(this, "Vergrößerungslimit Limit erreicht!", "Vergrößerungslimit Limit erreicht! Es wird die letzte Zoomstufe verwendet.");
            nscale = last_img->scale() ;
            midx = last_img->gaus_mid_re;
            midy = last_img->gaus_mid_im;
        }

       if(nscale > pow(10, 16)) {

           QMessageBox::warning(this, "Maximale Zoomtiefe erreicht!", "Aufgrund der limitierten Speichergröße der Standart Datentypen kann leider nicht tiefer gezoomt werden!");
//           nscale = last_img->scale();
//           midx = last_img->gaus_mid_re;
//           midy = last_img->gaus_mid_im;
       }

        ImageSetting *s = new ImageSetting((this->lastImgStructID = this->lastImgStructID + 1), nscale, midx, midy);
        s->init(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBoxMaxIterations->value(), ui->comboBox_Fraktal->currentIndex() == 0 /*is mandelbrot*/ );

        if(ui->comboBox_Fraktal->currentIndex() == 1) {
            s->juliaStart_img = last_img->juliaStart_img;
            s->juliaStart_real = last_img->juliaStart_real;
        }
        return s;
    }

}

void MainWindow::updateUiWithImageSetting(ImageSetting *imgs)
{
    if(noUpdateGui) {
        noUpdateGui = false;
        return;
    }

    // nach zurück oder home -> einstellungen laden
    this->ui->spinBoxW->setValue(imgs->img_w);
    this->ui->spinBoxH->setValue(imgs->img_h);
    this->ui->spinBoxMaxIterations->setValue(imgs->maxIterations);
    ui->comboBox_Fraktal->setCurrentIndex(imgs->isMandelbrotSet ? 0 : 1);
    ui->comboBox_palette->setCurrentIndex(imgs->farbalgorithmus);
    ui->radioButton_normalized->setChecked(imgs->normalized);
    ui->radioButton_invert->setChecked(imgs->inverted);

    ui->UseLogButton->setChecked(imgs->useLog);
    ui->logFak->setValue(imgs->log_multiplier);

    //Setzt Fraktal Farbe... Entweder Index

    this->setFratalColor(imgs->fraktalColor_index, imgs->fixFraktalColor);

    ui->spinBoxFarbWechselIntervalln->setValue( imgs->farbwechselIntervall );


    ui->radioButtonF1->setChecked(false);
    ui->radioButtonF2->setChecked(false);
    ui->radioButtonF3->setChecked(false);
    ui->radioButtonF4->setChecked(false);
    ui->radioButtonF5->setChecked(false);
    ui->radioButtonF6->setChecked(false);
    ui->radioButtonF7->setChecked(false);
    ui->radioButtonF8->setChecked(false);


    //set all used colors to spezific color
    for(auto & e : imgs->colors) {
        switch (e.first) {
        case 1:
            buttonColors[0] = e.second;
            ui->radioButtonF1->setChecked(true);

            break;
        case 2:
            buttonColors[1] = e.second;
            ui->radioButtonF2->setChecked(true);

            break;
        case 3:
            buttonColors[2] = e.second;
            ui->radioButtonF3->setChecked(true);

            break;
        case 4:
            buttonColors[3] = e.second;
            ui->radioButtonF4->setChecked(true);

            break;
        case 5:
            buttonColors[4] = e.second;
            ui->radioButtonF5->setChecked(true);
            break;

        case 6:
            buttonColors[5] = e.second;
            ui->radioButtonF6->setChecked(true);
            break;

        case 7:
            buttonColors[6] = e.second;
            ui->radioButtonF7->setChecked(true);
            break;

        case 8:
            buttonColors[7] = e.second;
            ui->radioButtonF8->setChecked(true);
            break;
        }

    }
    //update gui
    updateButtonColors();

    if(ui->comboBox_palette->currentText() == "<eigen>") {
        for(int i = 0; i < 8; i++)
            this->buttonColors_save[i] = buttonColors[i];

        this->updateIconInOwnColor(80);
    }

    ui->spinBoxHSV_satursion->setValue(imgs->hsv_saturation);
    ui->spinBoxHSV_value->setValue(imgs->spinBoxHSV_value);
    ui->spinBoxHSV_alpha->setValue(imgs->spinBoxHSV_alpha);
}

void MainWindow::startRefresh(ImageSetting *set, bool appendToList)
{
    if(set->scale() == 0) {
        perror("INVALID SETTING!");
        return;
    }

    //reset ui
    this->state = STATE::RUNNING;
//    ui->widget->setDisabled(true);
    ui->tab->setDisabled(true);
    ui->tab_2->setDisabled(true);
    ui->tab_3->setDisabled(true);
    ui->menuBar->setDisabled(true);
    ui->tab_4->setDisabled(true);

    ui->pushButtonStart->setText("Abbrechen");
    ui->progressBar->setEnabled(true);
    ui->frameButtons->setEnabled(false);

    // Reset Zahlenfolge
    zahlenfolge.removeZahlenfolge();

    //calc work for each thread
    size_t tc = std::max(std::min( ui->spinBox_threads->value(), (int)set->img_w), 1);
    ssize_t parts = set->img_w / tc;

    //fill image with background color:
    set->painter->fillRect(set->image->rect(), currentBackgroundColor);
    this->updateImage();

    //reset progress bar
    this->ui->progressBar->setMaximum(set->img_w);
    this->ui->progressBar->setValue(0);

    if(appendToList) {

        set->setColorSettings(this->ui->comboBox_palette->currentIndex(),
                             ui->radioButton_normalized->isChecked(),
                             log(log(ui->doubleSpinBoxEscapeR->value())),
                             ui->comboBoxMandelColor->currentIndex(),
                             this->getFraktalColor(),
                             ui->radioButton_invert->isChecked(),
                             ui->doubleSpinBoxEscapeR->value(),
                             ui->comboBox_intervall_absolut->currentIndex() == 0,
                             ui->UseLogButton->isChecked(),
                             ui->logFak->value());

        set->colors.clear();
        if(ui->radioButtonF1->isChecked())
            set->colors.push_back(QPair<int, QColor>(1, buttonColors[0]));
        if(ui->radioButtonF2->isChecked())
            set->colors.push_back(QPair<int, QColor>(2, buttonColors[1]));
        if(ui->radioButtonF3->isChecked())
            set->colors.push_back(QPair<int, QColor>(3, buttonColors[2]));
        if(ui->radioButtonF4->isChecked())
            set->colors.push_back(QPair<int, QColor>(4, buttonColors[3]));
        if(ui->radioButtonF5->isChecked())
            set->colors.push_back(QPair<int, QColor>(5, buttonColors[4]));
        if(ui->radioButtonF6->isChecked())
            set->colors.push_back(QPair<int, QColor>(6, buttonColors[5]));
        if(ui->radioButtonF7->isChecked())
            set->colors.push_back(QPair<int, QColor>(7, buttonColors[6]));
        if(ui->radioButtonF8->isChecked())
            set->colors.push_back(QPair<int, QColor>(8, buttonColors[7]));

        set->hsv_saturation = ui->spinBoxHSV_satursion->value();
        set->spinBoxHSV_value = ui->spinBoxHSV_value->value();
        set->spinBoxHSV_alpha = ui->spinBoxHSV_alpha->value();

        set->farbwechselIntervall = ui->spinBoxFarbWechselIntervalln->value();


        settingsList.push_back(set);
        currentImg = set;

    }

    qInfo() << "Start calculating new image...";


    //start workers
    for( size_t tn = 0; tn < tc; tn++) {
//        WorkerThread * wt = new WorkerThread(this);
//        this->tworkers.append( wt );

//        const ssize_t x_left_corner_part = (parts * tn);
//        const ssize_t x_right_corner_part = (parts * (tn + 1) + ( ( tn + 1 == tc ) ? (set->img_w - parts * (tn + 1)) : 0));
//        const ssize_t y_top_corner = 0;
//        const ssize_t y_bottom_corner = set->img_h;

//        //SETUP
//        wt->setRange(x_left_corner_part, x_right_corner_part, y_top_corner, y_bottom_corner);
//        //CONNECT
//        connect(wt, SIGNAL(finishedLine(QList<Pixel>*)), this, SLOT(finishedLine(QList<Pixel>*)));
//        connect(wt, SIGNAL(finished()), this, SLOT(threadFinished()));
//        //START
//        wt->startCalc(set, this->ui->comboBox_precession->currentIndex());

        // warte für den nächsten Thread
//        if(ui->spinBoxStartVerzoegerung->value()) {
//            QTime dieTime= QTime::currentTime().addMSecs(ui->spinBoxStartVerzoegerung->value());
//                while (QTime::currentTime() < dieTime)
//                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

                // Delay in milliseconds
                int delayMs = ui->spinBoxStartVerzoegerung->value();

                // Create a QTimer object
                QTimer* timer = new QTimer(this);

                // Set a single-shot timer with the desired delay
                timer->setSingleShot(true);
                timer->start(delayMs * tn);

                // Connect the timer's timeout signal to a slot or lambda function
                connect(timer, &QTimer::timeout, this, [=]() {
                    // This code will be executed after the delay
                    // Perform any desired actions here

                    WorkerThread * wt = new WorkerThread(this);
                    this->tworkers.append( wt );

                    const ssize_t x_left_corner_part = (parts * tn);
                    const ssize_t x_right_corner_part = (parts * (tn + 1) + ( ( tn + 1 == tc ) ? (set->img_w - parts * (tn + 1)) : 0));
                    const ssize_t y_top_corner = 0;
                    const ssize_t y_bottom_corner = set->img_h;

                    //SETUP
                    wt->setRange(x_left_corner_part, x_right_corner_part, y_top_corner, y_bottom_corner);
                    //CONNECT
                    connect(wt, SIGNAL(finishedLine(QList<Pixel>*)), this, SLOT(finishedLine(QList<Pixel>*)));
                    connect(wt, SIGNAL(finished()), this, SLOT(threadFinished()));
                    //START
                    wt->startCalc(set, this->ui->comboBox_precession->currentIndex());

#ifdef SINGLE_THREADTED_WEB_ASSEMBLY
                    wt->run();
#endif

                });
        }

//    }

}

#include <algorithm>


void MainWindow::afterColoring(ImageSetting *set)
{
    size_t total = 0;
    QImage img;
    this->ui->pushButtonStart->setDisabled(true);
    this->state = this->STATE::RUNNING;

    switch(set->farbalgorithmus) {
        case 13 ... 14:
        if( ! set->hueIsSetted() ) {
            ui->progressBar->setMaximum( set->img_w);
            ui->progressBar->setValue(0);
            QApplication::processEvents();

            for (size_t i = 0; i <= set->maxIterations; i++)
                total += set->getNumIterationsPerPixelAt(i);
            double ** hue = new double*[set->img_w];
            for(int i = 0; i < set->img_w; i++) {
                hue[i] = new double[set->img_h];
                memset(hue[i], 0, sizeof(double) * set->img_h );
            }

            for(ssize_t x = 0; x < set->img_w; x++) {
                QApplication::processEvents();

                for(ssize_t y = 0; y < set->img_h; y++) {
                    size_t iteration = set->getIterationCountAt(x, y);
                    for (size_t i = 0; i <= iteration; i++)
                        hue[x][y] += (double)set->getNumIterationsPerPixelAt(i) / (double)total; /* Must be floating-posize_t division. */
                }
                ui->progressBar->setValue(ui->progressBar->value() + 1);
            }
            set->setHue(hue, total);
            ui->progressBar->setValue(ui->progressBar->maximum());
        }
        for(ssize_t x = 0; x < set->img_w; x++) {
            QApplication::processEvents();

            for(ssize_t y = 0; y < set->img_h; y++) {
                if(set->getIterationCountAt(x, y) == set->maxIterations) {
                    set->painter->setPen( this->getFraktalColor() );
                } else {
                    set->painter->setPen(QColor::fromHsv(set->hue[x][y] * 359, set->hsv_saturation,
                                                               set->spinBoxHSV_value , set->spinBoxHSV_alpha));
                }
                set->painter->drawPoint(x, y);

            }
        }
        ui->progressBar->setValue(ui->progressBar->maximum());
        break;
    }

    if( ! img.isNull()) {
        set->image->swap(img);
        updateImage();
    }
    this->state = this->STATE::STOPED;
    this->ui->pushButtonStart->setDisabled(false);


}


void MainWindow::setOperationMode()
{

    // Wenn kein Fraktalwechses vorliegt
    if((currentImg->isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 0) || (!currentImg->isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 1)) {
        // Wenn zoom und einstellungen verändert worden sind
        if(editedSettings && zoomRect.isShown()) {
            ui->pushButtonStart->setText("Vergrößern mit neuen Einstellungen");
            this->op_mode = MainWindow::OP_MODE::APPLY_SETTINGS_AND_ZOOM;

        } else if( editedSettings ) {
            ui->pushButtonStart->setText("Einstellungen übernehmen");
            this->op_mode = MainWindow::OP_MODE::APPLY_SETTINGS;

        } else if( zoomRect.isShown() ) {
            ui->pushButtonStart->setText("Ausschnitt vergrößern");
            this->op_mode = MainWindow::OP_MODE::ZOOM_TO_SELECTED;

        } else {
            //nichts verändert --> refresh
            ui->pushButtonStart->setText("Bild neu laden");
            this->op_mode = MainWindow::OP_MODE::REFRESH;

        }

        // Wechsel zu Mandelbrot Menge
    } else if((!currentImg->isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 0)) {
        ui->pushButtonStart->setText("Zurück zum Apfelmänchen");
        this->op_mode = MainWindow::OP_MODE::BACK_TO_MANDELBROT;

        // zu fraktal
    } else {
        ui->pushButtonStart->setText("Julia-Menge an Mausposition berechnen");
        this->op_mode = MainWindow::OP_MODE::CALC_JULIA;
    }

    if (imgSerie.imgCount > 0) {
        ui->pushButtonStart->setText("Bilder Serie an Mausposition starten");
    }

    this->updateImage();


}


void MainWindow::endRefresh(bool appendToListHistory)
{
    keyPressed[Qt::LeftButton] = false;
    keyPressed[Qt::RightButton] = false;

    stopThreads();


    //Coloring ---> histogramm
    if(!this->isBackOrVor || (this->isBackOrVor && ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked()))
        afterColoring(currentImg);
    updateImage();


    this->state = STATE::STOPED;

    this->zoomRect.hide();
    this->zahlenfolge.removeZahlenfolge();

    if(this->currentImg->id == settingsList.at(0)->id) {
        this->ui->pushButtonHome->setText("Reset");
        // sonderfall reload... auch vor nach refresh möglich!
        if(settingsList.length() > 1) {
                ui->pushButton_vor->setDisabled(false);
        } else
            ui->pushButton_vor->setDisabled(true);
        ui->pushButton_back->setEnabled(false);
    } else {
        this->ui->pushButtonHome->setText("⌂ Home");
        // sonderfall reload... auch vor nach refresh möglich!
        if(settingsList.length() > 1)
            ui->pushButton_back->setEnabled(true);
        else
            ui->pushButton_back->setEnabled(false);
        if(this->currentImg->id == settingsList.last()->id)
            ui->pushButton_vor->setDisabled(true);
        else
            ui->pushButton_vor->setDisabled(false);
    }

    ui->progressBar->setEnabled(false);
    ui->frameButtons->setEnabled(true);

    //update dpi if aut...e.g. moved to display with diffrent dpi
    if(ui->comboBox->currentText() == "auto")
        this->on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());

    ui->lineEditScaleAbs->setText("1: " + QString::number((double)currentImg->scale()));
    if(currentImg->isMandelbrotSet) {
        ui->label_julia_c->setText("-");
        ui->label_julia_c_im->setText("-");
    } else {
        ui->label_julia_c->setText(QString::number( (double)(currentImg->juliaStart_real), 'g', 10 )) ;
        ui->label_julia_c_im->setText(QString::number( (double)(-currentImg->juliaStart_img), 'g', 10 ) + " i");
    }

    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->bmRe->setText(QString::number(this->currentImg->gaus_mid_re, 'g', 12));
    ui->bmIm->setText(QString::number( - this->currentImg->gaus_mid_im, 'g', 12) + " i");



    if( ui->actionAutoscroll_in_Mitte->isChecked() ) {
        qDebug() << "Center..." << ui->scrollArea->verticalScrollBar()->maximum() / 2 << " max: " << ui->scrollArea->verticalScrollBar()->maximum();

        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum() / 2);
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->maximum() / 2);

    }

    if(appendToListHistory && !this->isBackOrVor ) {
        auto i = new QListWidgetItem( QString(currentImg->isMandelbrotSet ? "M" : "J" ) + ": Re(" + QString::number(this->currentImg->gaus_mid_re) +
                                     ") Im(" +  QString::number( - this->currentImg->gaus_mid_im ) + " i)");
        i->setIcon(QIcon(QPixmap::fromImage(*this->currentImg->image).scaled(QSize(256, 256), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::FastTransformation)));
        i->setData(187, currentImg->id);

        if(ui->listWidgetHistory->count() == 0) {
            i->setText("⌂ Home");
        }

        ui->listWidgetHistory->addItem(i);
        ui->listWidgetHistory->setCurrentItem(i);
    }

    this->updateUiWithImageSetting( currentImg );

    if(!this->isBackOrVor) {
        this->editedSettings = false;
    } else {
        this->editedSettings = true;
        this->isBackOrVor = false;
    }
    this->setOperationMode();

//    ui->widget->setDisabled(false);
    ui->tab->setDisabled(false);
    ui->tab_2->setDisabled(false);
    ui->tab_3->setDisabled(false);
    ui->tab_4->setDisabled(false);
    ui->menuBar->setDisabled(false);


    //Img Serie
    useOldPosForImgSerie = false;
    if(imgSerie.imgCount > 0) {
        // save img: ok

        QImageWriter imgWriter;
        imgWriter.setFileName(imgSerie.dirPath + "/" + imgSerie.prefix + QString::number( imgSerie.nameItStart++ ) + imgSerie.suffix);
        if( ! imgWriter.write( /*ui->radioButtonsaveedits->isChecked() ? *this->currentImg->image : */ ui->imageView->getImg() ))
            imgSerie.imgCount = 0;

        // LAST IMG
        if(imgSerie.imgCount == 1) {

            //Create Video?

        }

        // no save to history: ok
        // set zoom: ok
        imgSerie.imgCount--;
        this->ui->statusbar->showMessage("Erstelle Bilderfolge ( " + QString::number(imgSerie.imgCountConst - imgSerie.imgCount) + " / " + QString::number(imgSerie.imgCountConst) +" )...", 4000);
        if(imgSerie.imgCount > 0) {
            //Add ITC
            ui->spinBoxMaxIterations->setValue( ui->spinBoxMaxIterations->value() + imgSerie.addIT );
            //add log faktor
            ui->logFak->setValue( ui->logFak->value() + imgSerie.log_faktor_add );

            // clear History [-2]!

            for(int i = 1; i < settingsList.length() - 2; i++) {
                auto s = settingsList.takeAt(i--);
                s->cleanUP();
                delete s;
            }

            for(int i = 1; i < ui->listWidgetHistory->count() - 2; i++)
                delete ui->listWidgetHistory->takeItem(i--);

            useOldPosForImgSerie = true;
            this->zoomRect.show();
            this->setOperationMode();
            // Imitate "click"
            this->ui->pushButtonStart->click();

        }
    }
}


void MainWindow::updateImage()
{

//    if(currentImg->image) {
//        ui->imageView->setImage(*currentImg->image);
//        if(this->fullScreenView->isVisible())
//            this->fullScreenView->setImage(*this->currentImg->image);
//    }
    this->update();

    /*Bei default theme comischer bug -> bild wird nur bei enterEvent akutualisiert, voer bug bei anderen modi...so mit extra update gehts lol*/
    // * nur bei qdarkstyle buggts..alle anderen ...z.b. fusion updaten auch nicht!
    if(ui->comboBox_theme->currentIndex() < 4) {
        ui->imageView->update();
    }
}

void MainWindow::startImgFolge()
{
    DialogImageSerie dIm(this, this->imgSerie, this->currentImg->scale(), ui->spinBoxMaxIterations->value(), ui->logFak->value());
    if( dIm.exec() == QDialog::Accepted ) {
        this->imgSerie = dIm.getImgSerieSettings();
        this->ui->spinBox_zoom->setValue( imgSerie.zoomStep );
        this->setOperationMode();
    } else {
        this->imgSerie.imgCount = 0;
        this->useOldPosForImgSerie = false;
        this->setOperationMode();
        this->ui->spinBox_zoom->setValue( 2.00 );
    }
}

void MainWindow::createVideo()
{
    DialogCreateVideo dvi(this, imgSerie);
    if(dvi.exec() == QDialog::Accepted) {

        QAviWriter writer(dvi.getFilePath() , QSize(currentImg->img_w, currentImg->img_h), dvi.getFPS(), dvi.getCodec());
        if( QFile(dvi.getWaveAudio()).exists() )
            writer.setAudioFileName(dvi.getWaveAudio()); // set audio track

        writer.open();
        for(int i = dvi.getNameItStart(); i <= dvi.getNameItEnd(); i++) {
            qDebug() << dvi.getImgsPath() + ( dvi.getImgsPath().endsWith("/") ? "" : "/" ) + dvi.getPrefix() + QString::number(i) + dvi.getSuffix();
            this->ui->statusbar->showMessage("[ " + QString::number(i) + " / " + QString::number( dvi.getNameItEnd()) + " ]: '"
                                             + dvi.getImgsPath() + ( dvi.getImgsPath().endsWith("/") ? "" : "/" ) + dvi.getPrefix() + QString::number(i) + dvi.getSuffix() + "'...", 4000);
            QApplication::processEvents();

            QImage img = QImage(  dvi.getImgsPath() + ( dvi.getImgsPath().endsWith("/") ? "" : "/" ) + dvi.getPrefix() + QString::number(i) + dvi.getSuffix());
            if(!img.isNull()) {
                writer.addFrame(img);
            } else {
                if( QMessageBox::warning(this, "Bild konnte nicht geladen werden!", "Die Datei '" + dvi.getImgsPath()
                                     + ( dvi.getImgsPath().endsWith("/") ? "" : "/" ) + dvi.getPrefix() + QString::number(i) + dvi.getSuffix() + "' existiert nicht oder konnte nicht geladen werden!",
                                     QMessageBox::StandardButton::Abort | QMessageBox::StandardButton::Ignore  ) == QMessageBox::StandardButton::Abort)
                    break;
            }
        }
        writer.close();


    }
}



bool MainWindow::checkForFinished()
{
    if(this->ui->progressBar->value() == this->ui->progressBar->maximum() ) {
        for ( ssize_t i = 0; i < this->tworkers.length(); i++ ) {
            if (this->tworkers.at(i)->isFinished()
#ifdef SINGLE_THREADTED_WEB_ASSEMBLY
                    || true
#endif
                ) {
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
    /*
     * MOVE TO DESTRUCTOR!!
     *
    for ( ssize_t i = 0; i <  this->tworkers.length(); i++ ) {
        if (this->tworkers.at(i)->isRunning()) {
            this->tworkers.at(i)->quit();
            if(!this->tworkers.at(i)->wait(100)) {
                this->tworkers.at(i)->terminate();
            }
        }
    }

    */

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


void MainWindow::finishedLine(QList<Pixel> *list)
{
    for( ssize_t l = 0; l < list->size(); l++) {
        currentImg->painter->setPen(list->at(l).color);

        ssize_t x = list->at(l).c_x,
                y = list->at(l).c_y;
        currentImg->painter->drawPoint(x, y);

        if(x >= 0 && x < currentImg->img_w && y >= 0 && y < currentImg->img_h) {
            currentImg->setIterationCountAt(x, y, list->at(l).iters);
            currentImg->iterations_normal[x][y] = list->at(l).normalized_iters;
            //Histogramm
            currentImg->addNumIterationsPerPixel(list->at(l).iters);
        }

    }
    this->ui->progressBar->setValue(this->ui->progressBar->value() + 1);
    delete list;
    imgUpdateNeeded = true;
}



void MainWindow::threadFinished()
{
//    WorkerThread * sender_obj = qobject_cast<WorkerThread*>(QObject::sender());
//    this->tworkers.removeAll(sender_obj);
//    if(sender_obj)
//        delete sender_obj;
}



#define TINYEXR_USE_THREAD 1
//#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_IMPLEMENTATION
#include "openexr/tinyexr.h"

float* convertQImageToFloatArray(const QImage& inputImage, int& width, int& height) {
    width = inputImage.width();
    height = inputImage.height();

    float* rgbData = new float[width * height * 3];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgba64 pixel = inputImage.pixelColor(x, y).rgba64();


            //HERE Pos processing effects, e.g. bloom!!!!!!


            float r = /*pow( (1.7**/ (float)pixel.red()/*)*/   / 65535.0f/*, 3.)*/; // Scale to [0, 1]
            float g = /*pow( (1.7**/ (float)pixel.green()/*)*/ / 65535.0f/*, 3.)*/; // Scale to [0, 1]
            float b = /*pow( (1.7**/ (float)pixel.blue()/*)*/  / 65535.0f/*, 3.)*/; // Scale to [0, 1]

            // Pre-multiplied alpha
            rgbData[(y * width + x) * 3 + 0] = r ;
            rgbData[(y * width + x) * 3 + 1] = g ;
            rgbData[(y * width + x) * 3 + 2] = b;
        }
    }

    return rgbData;
}


// See `examples/rgbe2exr/` for more details.
bool SaveEXR(float* rgb, int width, int height, const char* outfilename) {
    if(!rgb)
        return false;

    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 3;

    std::vector<float> images[3];
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    // Split RGBRGBRGB... into R, G and B layer
    for (int i = 0; i < width * height; i++) {
        images[0][i] = rgb[3*i+0];
        images[1][i] = rgb[3*i+1];
        images[2][i] = rgb[3*i+2];
    }

    float* image_ptr[3];
    image_ptr[0] = &(images[2].at(0)); // B
    image_ptr[1] = &(images[1].at(0)); // G
    image_ptr[2] = &(images[0].at(0)); // R

    image.images = (unsigned char**)image_ptr;
    image.width = width;
    image.height = height;

    header.num_channels = 3;
    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
    // Must be (A)BGR order, since most of EXR viewers expect this channel order.
    strncpy(header.channels[0].name, "B", 255); header.channels[0].name[strlen("B")] = '\0';
    strncpy(header.channels[1].name, "G", 255); header.channels[1].name[strlen("G")] = '\0';
    strncpy(header.channels[2].name, "R", 255); header.channels[2].name[strlen("R")] = '\0';

    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    for (int i = 0; i < header.num_channels; i++) {
        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
    }

    const char* err = NULL; // or nullptr in C++11 or later.
    int ret = SaveEXRImageToFile(&image, &header, outfilename, &err);
    if (ret != TINYEXR_SUCCESS) {
        fprintf(stderr, "Save EXR err: %s\n", err);
        FreeEXRErrorMessage(err); // free's buffer for an error message
        free(header.channels);
        free(header.pixel_types);
        free(header.requested_pixel_types);
        return ret;
    }
    printf("Saved exr file. [ %s ] \n", outfilename);

    delete []rgb;

    free(header.channels);
    free(header.pixel_types);
    free(header.requested_pixel_types);
    return 0;
}


void MainWindow::pushButtonSaveImg_clicked()
{

    QImageWriter imgWriter;
    QString info = QString(currentImg->isMandelbrotSet ? "Mandelbrot-Menge" : "Julia-Menge")
            + " Re(" + QString::number(this->currentImg->gaus_mid_re) + ") Im(" + QString::number(this->currentImg->gaus_mid_im) + ") Scale(1 zu " + QString::number(currentImg->scale()) + ")";
    QString filename = QFileDialog::getSaveFileName(this, "Bild speichern - Dateiendung nach gewünschtem Format ändern. Standart: .png", "", "Images (*.png *.xpm *.jpg)");


    if( ! filename.isEmpty() && !filename.endsWith(".exr")) {
        currentImg->image->setText("Description", info);
        filename = filename + ((!filename.contains(".")) ? ".png" : "");
        filename.insert(filename.lastIndexOf("."), " - " + info);
        imgWriter.setFileName(filename);

        if( ! imgWriter.write( /*ui->radioButtonsaveedits->isChecked() ? *this->currentImg->image : */ ui->imageView->getImg() ))
            QMessageBox::warning(this, "Speichern fehlgeschlagen!", "Das Bild '" + filename + "' konnte nicht gespeichert werden! Haben sie die Dateiendung vergessen?");
        else
            this->ui->statusbar->showMessage("Bild {Re(" + QString::number(this->currentImg->gaus_mid_re) + ") Im(" + QString::number(this->currentImg->gaus_mid_im) + ") 1:"
                                             +  QString::number(currentImg->scale()) + " [" + QString::number(currentImg->img_w) + "x" + QString::number(currentImg->img_h) + "]} gespeichert unter '" + filename + "'." , 5000);
    } else if(filename.endsWith(".exr")){
        int w, h;
        float * img_buffer = convertQImageToFloatArray(ui->imageView->getImg(), w, h);
        if(SaveEXR(img_buffer, w, h, filename.toStdString().c_str()))
            this->ui->statusbar->showMessage("Fehler: Kein Bild gespeichert!", 1000);

    }else{
        this->ui->statusbar->showMessage("Kein Bild gespeichert!", 1000);
    }



}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    zustandWechseln("mousePressEvent", "NOT_IN_IMG", QPoint(), event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *)
{

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    zustandWechseln("mouseReleaseEvent", "NOT_IN_IMG", QPoint(), event);
}

void MainWindow::mouse_move_in_img(QPoint pos)
{
    zustandWechseln("mouseMoveEvent", "", pos);
}

void MainWindow::mouse_press_in_img(QMouseEvent *event, QPoint pos)
{
    zustandWechseln("mousePressEvent", "", pos, event);
}

void MainWindow::mouse_release_in_img(QMouseEvent *event, QPoint pos)
{
    zustandWechseln("mouseReleaseEvent", "", pos, event);
}

void MainWindow::mouse_double_click_in_img(QMouseEvent *event, QPoint pos)
{
    zustandWechseln("DOPPELKLICK_IN_BILD", "", pos, event);
}

void MainWindow::key_press_in_img(QKeyEvent *ev)
{
    this->keyPressEvent(ev);
}

void MainWindow::on_pushButton_vor_clicked()
{
    zustandWechseln("VOR");
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QImage img = *currentImg->image;
    QPainter painter;
    painter.begin(&img);
    painter.setRenderHint(QPainter::Antialiasing, this->ui->actionAntialising->isChecked());

    switch (state) {
    case MainWindow::STOPED:

        if(zoomRect.isShown() || zahlenfolge.isShown() || this->ui->radioButtonKoords_2->isChecked()) {

            // Wenn Bilderfolge, dann zoomRect is Shown, aber soll kein Quadrat sein, sondern ein Punkt!
            if(imgSerie.imgCount > 0) {
                painter.setPen(Qt::white);
                painter.drawPoint(zoomRect.getMousePos());
                painter.drawEllipse(zoomRect.getMousePos(), 5, 5);

                painter.setPen(Qt::black);
                painter.drawEllipse(zoomRect.getMousePos(), 2, 2);
                painter.drawEllipse(zoomRect.getMousePos(), 6, 6);



            } else if(zoomRect.isShown()) {
                if(!( currentImg->isMandelbrotSet && ui->comboBox_Fraktal->currentIndex() == 1)) {
                    painter.setPen(Qt::black);
                    painter.drawRect(zoomRect.getRect());
                    painter.setPen(Qt::white);
                    //                painter.drawRect(QRect(x_left_corner - 1, y_left_corner - 1, w + 2, h + 2));
                    painter.drawRect(zoomRect.getRect().adjusted(-1, -1, +1, +1));
                } else {

                    painter.setPen(QPen(Qt::black, 2));
                    painter.drawEllipse(zoomRect.getMousePos(), 4, 4);
                    painter.setPen(QPen(Qt::white, 2));
                    painter.drawEllipse(zoomRect.getMousePos(), 5, 5);
                    painter.drawPoint(zoomRect.getMousePos());

                }

            }

            if( ui->groupBox_2->isChecked() && zahlenfolge.isShown()) {
                painter.setPen(QPen( QColor(ui->comboBox_color_zahlenfolge->currentText()), ui->spinBoxZahlenfolgeLinienbreite->value()) );
                if( ui->comboBox_drawStyle_zahlenfolge->currentIndex() == 0) {
                    painter.drawPoints(zahlenfolge.getZahlenfolge().toVector().data(), zahlenfolge.getZahlenfolge().length());
                } else {
                    auto list = zahlenfolge.getZahlenfolge();
                    QPoint last = QPoint();
                    if(list.length() > 0) {
                        painter.drawPoint(list.first());
                        last = list.first();
                    }
                    for( int i = 1; i < list.length(); i++ ) {
                        painter.drawLine(last, list.at(i));
                        if(last != list.at(i))
                            last = list.at(i);
                        if(ui->comboBox_drawStyle_zahlenfolge->currentIndex() == 2)
                            painter.drawEllipse(last, 3, 3);

                    }


                }
            }


            // optimieren -> nur sichtbares zeug berechnen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(this->ui->radioButtonKoords_2->isChecked() ) {

                QPoint startX = QPoint(currentImg->mapGausReToImgPos( ui->doubleSpinBox_Koord_x_start->value()  ), currentImg->mapGausImToImgPos(0)); // QPoint(50, img_h / 2);
                QPoint endeX =  QPoint(currentImg->mapGausReToImgPos(ui->doubleSpinBox_Koord_x_end->value()), currentImg->mapGausImToImgPos(0)); // QPoint (img_w - 50, startX.y());

                QPoint startY = QPoint(currentImg->mapGausReToImgPos(0), currentImg->mapGausImToImgPos(-ui->doubleSpinBox_Koord_y_end->value())); //QPoint (img_w / 2, 50);
                QPoint endeY = QPoint(currentImg->mapGausReToImgPos(0), currentImg->mapGausImToImgPos(-ui->doubleSpinBox_Koord_y_start->value()));  //QPoint (startY.x(), img_h - 50 );

                QLine xA = QLine(startX, endeX);
                QLine yA = QLine(startY, endeY);
                QColor color = QColor(ui->comboBoxColorKoordSystem->currentText());

                painter.setPen(QPen(color, ui->spinBoxKoordPenThickness->value()));



                QFont font = painter.font() ;
                font.setPointSize(8 + 2 * ui->spinBoxKoordPenThickness->value() );
                painter.setFont(font);

                //Achsen
                painter.drawLine(xA);
                painter.drawLine(yA);

                //Pfeile
                painter.drawLine(xA.p2(), xA.p2() + QPoint(-5, -5));
                painter.drawLine(xA.p2(), xA.p2() + QPoint(-5, 5));

                painter.drawLine(yA.p1(), yA.p1() - QPoint(-5, -5));
                painter.drawLine(yA.p1(), yA.p1() - QPoint(5, -5));



                if(this->ui->actionAchsenbeschriftungen_Anzeigen_2->isChecked() && ui->spinBoxBeschriftungen->value() && ui->spinBoxBeschriftungenY->value()) {
                    //Beschriftung


                    painter.drawText(yA.p1() + QPoint(10, 5), "Im(c)" );
                    painter.drawText(xA.p2() + QPoint(-15, -10), "Re(c)");



                    double x_step = ui->spinBoxBeschriftungen->value();
                    double minX = currentImg->mapImgPosReToGaus(xA.x1());
                    double maxX = currentImg->mapImgPosReToGaus(xA.x2());
                    double x = x_step;
                    for(int i = 0 ; i < 9999 && x < maxX; i++) {
                        int x_pos = currentImg->mapGausReToImgPos(x);
                        painter.drawLine(QPoint(x_pos, xA.y1() -5), QPoint(x_pos, xA.y1() + 5));
                        painter.drawText(QPoint(x_pos - 6, xA.y1() - 10), QString::number( x  ));
                        x += x_step;
                    }

                    x = - x_step;
                    for(int i = 0 ; i < 9999 && x > minX; i++) {
                        int x_pos = currentImg->mapGausReToImgPos(x);
                        painter.drawLine(QPoint(x_pos, xA.y1() -5), QPoint(x_pos, xA.y1() + 5));
                        painter.drawText(QPoint(x_pos - 6, xA.y1() - 10), QString::number(  x  ));
                        x -= x_step;
                    }

                    double y_step = ui->spinBoxBeschriftungenY->value();
                    double minY = currentImg->mapImgPosImToGaus(yA.y1());
                    double maxY = currentImg->mapImgPosImToGaus(yA.y2());
                    double y = y_step;
                    for(int i = 0 ; i < 9999 && y < maxY; i++ ) {
                        int y_pos = currentImg->mapGausImToImgPos(y);
//                        qDebug() << y_pos;
//                        if(y_pos + 5 > yA.x2())
//                            break;
                        painter.drawLine(QPoint(yA.x1() - 5, y_pos ), QPoint(yA.x1() + 5, y_pos ));
                        painter.drawText(QPoint(yA.x1() + 10, y_pos + 5), QString::number( y  ) + "i");
                        y += y_step;
                    }
                    y = -y_step;
                    for(int i = 0 ; i < 9999 && y > minY; i++ ) {
                        int y_pos = currentImg->mapGausImToImgPos(y);
                        painter.drawLine(QPoint(yA.x1() - 5, y_pos ), QPoint(yA.x1() + 5, y_pos ));
                        painter.drawText(QPoint(yA.x1() + 10, y_pos + 5), QString::number( -y  ) + "i");
                        y -= y_step;
                    }

                }

            }

//            painter.end();
//            QPainter painter2(ui->labelFraktal);
//            this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));
//            painter2.drawImage(0, 0, img);
//            painter2.end();
            //this->ui->labelFraktal->setPixmap(QPixmap::fromImage(img));


        }

        break;
    case MainWindow::RUNNING:
        break;

    }


    painter.end();
    ui->imageView->setImage(img);
    if(fullScreenView->isVisible()) {
        fullScreenView->setImage(img);
    }

}


void MainWindow::setFullScreenWindowVisible(bool state)
{
    if(state) {
        this->fullScreenView->setImage(*this->currentImg->image);
        this->fullScreenView->showFullScreen();
        this->ui->actionVollbildfenster_anzeigen->setChecked(true);

    } else {
        this->fullScreenView->hide();
        this->ui->actionVollbildfenster_anzeigen->setChecked(false);
    }
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event << " --> ";
    switch (event->key()) {
    case Qt::Key_Plus:
        this->ui->spinBox_zoom->setValue( this->ui->spinBox_zoom->value() + 1 );
        break;
    case Qt::Key_Minus:
        this->ui->spinBox_zoom->setValue( this->ui->spinBox_zoom->value() - 1 );
        break;


    case Qt::Key_1:
        this->ui->comboBox_palette->setCurrentIndex(1);
        break;
    case Qt::Key_2:
        this->ui->comboBox_palette->setCurrentIndex(2);
        break;
    case Qt::Key_3:
        this->ui->comboBox_palette->setCurrentIndex(3);
        break;
    case Qt::Key_4:
        this->ui->comboBox_palette->setCurrentIndex(4);
        break;
    case Qt::Key_5:
        this->ui->comboBox_palette->setCurrentIndex(5);
        break;
    case Qt::Key_6:
        this->ui->comboBox_palette->setCurrentIndex(6);
        break;
    case Qt::Key_7:
        this->ui->comboBox_palette->setCurrentIndex(7);
        break;
    case Qt::Key_8:
        this->ui->comboBox_palette->setCurrentIndex(8);
        break;
    case Qt::Key_9:
        this->ui->comboBox_palette->setCurrentIndex(9);
        break;

    case Qt::Key_X:
        this->ui->comboBox_palette->setCurrentIndex(11);
        break;
    case Qt::Key_Y:
        this->ui->comboBox_palette->setCurrentIndex(12);
        break;

    case Qt::Key_Z:
        this->ui->comboBox_palette->setCurrentIndex(14);
        break;


    case Qt::Key_B:
        this->ui->pushButton_back->click();
        break;
    case Qt::Key_N:
        this->ui->pushButton_vor->click();
        break;

    case Qt::Key_Escape:
    case Qt::Key_Q:
    case Qt::Key_L:
    case Qt::Key_E:
    case Qt::Key_Back:
        if(this->fullScreenView->isVisible())
            this->setFullScreenWindowVisible(false);
        break;


    case Qt::Key_I:
        this->startImgFolge();
        break;
    case Qt::Key_O:
        this->createVideo();
        break;

    case Qt::Key_H: {
        ssize_t n_v = ui->spinBoxMaxIterations->value() + ui->spinBoxMaxIterations->singleStep();
        if(n_v <= ui->spinBoxMaxIterations->maximum())
            ui->spinBoxMaxIterations->setValue(n_v);
        break;
        }
    case Qt::Key_T: {
        ssize_t n_v = ui->spinBoxMaxIterations->value() - ui->spinBoxMaxIterations->singleStep();
        if(n_v >= ui->spinBoxMaxIterations->minimum())
            ui->spinBoxMaxIterations->setValue(n_v);
        break;
        }
    case Qt::Key_Control:
        this->ui->imageView->setControlKeyIsPressed(true);
        break;

    default:
        break;
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Control) {
        this->ui->imageView->setControlKeyIsPressed(false);

       // ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum() / 2);
       // ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->maximum() / 2);
        // QPoint cursorInScrollArea = ui->scrollArea->mapFromGlobal(QCursor::pos());

        // // Get current scroll values and the maximum scroll values
        // int hScrollValue = ui->scrollArea->horizontalScrollBar()->value();
        // int vScrollValue = ui->scrollArea->verticalScrollBar()->value();
        // int hMaxScroll = ui->scrollArea->horizontalScrollBar()->maximum();
        // int vMaxScroll = ui->scrollArea->verticalScrollBar()->maximum();

        // // Calculate the new scroll values to center the cursor
        // int newHScrollValue = hScrollValue + cursorInScrollArea.x() - (ui->scrollArea->viewport()->width() / 2);
        // int newVScrollValue = vScrollValue + cursorInScrollArea.y() - (ui->scrollArea->viewport()->height() / 2);

        // // Ensure the new scroll values are within the scroll range
        // newHScrollValue = qBound(0, newHScrollValue, hMaxScroll);
        // newVScrollValue = qBound(0, newVScrollValue, vMaxScroll);

        // // Set the new scroll values
        // ui->scrollArea->horizontalScrollBar()->setValue(newHScrollValue);
        // ui->scrollArea->verticalScrollBar()->setValue(newVScrollValue);
    }
}




void MainWindow::on_spinBoxMaxIterations_valueChanged(int)
{
    this->editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_comboBox_palette_currentIndexChanged(int index)
{
    this->editedSettings = true;
    this->setOperationMode();

    //Setzte Standart Einstellungen
    ui->radioButton_normalized->setEnabled(true);
    ui->radioButton_invert->setEnabled(true);
    ui->stackedWidgetExtraSettings->setCurrentIndex(0);
    for(int i = 0; i < 8; i++)
        buttonColors[i] = QColor::fromRgb(255,255,255);
    //Hier dont_update_save_check_buttons setzten, weil wenn eigens ausgewäöhlt und setChecked gesetzt wird, wird save_überschrieben!
    dont_update_save_check_buttons = true;
    ui->radioButtonF1->setChecked(false);
    ui->radioButtonF2->setChecked(false);
    ui->radioButtonF3->setChecked(false);
    ui->radioButtonF4->setChecked(false);
    ui->radioButtonF5->setChecked(false);
    ui->radioButtonF6->setChecked(false);
    ui->radioButtonF7->setChecked(false);
    ui->radioButtonF8->setChecked(false);
    dont_update_save_check_buttons = false;

    //Ändere für spezifische Einstellung
    switch(index) {

    //RGB Farbverlauf:
    case 0:
        this->ui->comboBox_palette->setCurrentIndex(1);
        break;
    case 1: //EIGEN
        // lade gespeicherte farben
        for(int i = 0; i < 8; i++)
            this->buttonColors[i] = buttonColors_save[i];
        ui->radioButtonF1->setChecked(isChecked_save[0]);
        ui->radioButtonF2->setChecked(isChecked_save[1]);
        ui->radioButtonF3->setChecked(isChecked_save[2]);
        ui->radioButtonF4->setChecked(isChecked_save[3]);
        ui->radioButtonF5->setChecked(isChecked_save[4]);
        ui->radioButtonF6->setChecked(isChecked_save[5]);
        ui->radioButtonF7->setChecked(isChecked_save[6]);
        ui->radioButtonF8->setChecked(isChecked_save[7]);
        break;
    case 2:
        if(default_colors.length() > 2)
            for(int i = 0; i < default_colors.at(2).length(); i++)
                buttonColors[i] = default_colors.at(2).at(i);
        ui->radioButtonF1->setChecked(true);
        break;
    case 3:
        if(default_colors.length() > 3)
            for(int i = 0; i < default_colors.at(3).length(); i++)
                buttonColors[i] = default_colors.at(3).at(i);
        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        break;
    case 4:
        if(default_colors.length() > 4)
            for(int i = 0; i < default_colors.at(4).length(); i++)
                buttonColors[i] = default_colors.at(4).at(i);

        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        ui->radioButtonF3->setChecked(true);
        ui->radioButtonF4->setChecked(true);
        ui->radioButtonF5->setChecked(true);
        ui->radioButtonF6->setChecked(true);
        ui->radioButtonF7->setChecked(true);
        break;
    case 5:
        if(default_colors.length() > 5)
            for(int i = 0; i < default_colors.at(5).length(); i++)
                buttonColors[i] = default_colors.at(5).at(i);
        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        ui->radioButtonF3->setChecked(true);
        ui->radioButtonF4->setChecked(true);

        break;
    case 6:
        if(default_colors.length() > 6)
            for(int i = 0; i < default_colors.at(6).length(); i++)
                buttonColors[i] = default_colors.at(6).at(i);
        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        ui->radioButtonF3->setChecked(true);
        break;
    case 7:
        if(default_colors.length() > 7)
            for(int i = 0; i < default_colors.at(7).length(); i++)
                buttonColors[i] = default_colors.at(7).at(i);
        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        ui->radioButtonF3->setChecked(true);
        ui->radioButtonF4->setChecked(true);
        ui->radioButtonF5->setChecked(true);
        ui->radioButtonF6->setChecked(true);
        ui->radioButtonF7->setChecked(true);
        ui->radioButtonF8->setChecked(true);
        break;
    case 8:
        if(default_colors.length() > 8)
            for(int i = 0; i < default_colors.at(8).length(); i++)
                buttonColors[i] = default_colors.at(8).at(i);
        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        ui->radioButtonF3->setChecked(true);
        ui->radioButtonF4->setChecked(true);
        ui->radioButtonF5->setChecked(true);
        ui->radioButtonF6->setChecked(true);
        ui->radioButtonF7->setChecked(true);
        ui->radioButtonF8->setChecked(true);
        break;
    case 9:
        if(default_colors.length() > 9)
            for(int i = 0; i < default_colors.at(9).length(); i++)
                buttonColors[i] = default_colors.at(9).at(i);
        ui->radioButtonF1->setChecked(true);
        ui->radioButtonF2->setChecked(true);
        break;

    //HSV
    case 10:
        this->ui->comboBox_palette->setCurrentIndex(11);
        break;
    case 11:
//        ui->stackedWidgetExtraSettings->setCurrentIndex(1);
//        ui->stackedWidgetExtraSettings->setVisible(true);
//        break;
    case 12:
        ui->stackedWidgetExtraSettings->setCurrentIndex(1);
        ui->stackedWidgetExtraSettings->setVisible(true);
        break;

    //Histogramm
    case 13:
        this->ui->comboBox_palette->setCurrentIndex(14);
        break;
    case 14:
        ui->radioButton_normalized->setEnabled(false);
        ui->stackedWidgetExtraSettings->setCurrentIndex(1);
        ui->radioButton_invert->setEnabled(false);

        break;

    }

    //Farben der Buttons  akutualisieren
    this->updateButtonColors();
    if(ui->comboBox_palette->currentText() == "<eigen>") {
        this->updateIconInOwnColor(80);
    }

}


void MainWindow::on_radioButton_normalized_toggled(bool)
{
    this->editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_radioButton_invert_toggled(bool)
{
    this->editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_spinBoxW_valueChanged(int)
{
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    this->editedSettings = true;
    this->setOperationMode();
    this->updateImage();
}


void MainWindow::on_spinBoxH_valueChanged(int)
{
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
    this->editedSettings = true;
    this->setOperationMode();
    this->updateImage();
}


void MainWindow::on_groupBoxMandelFarbe_toggled(bool)
{
    this->editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_comboBoxMandelColor_currentIndexChanged(int)
{
    this->editedSettings = true;
    this->setOperationMode();
    if(ui->comboBoxMandelColor->currentText() == "<andere>") {
        ui->pushButtonFraktalColor->setVisible(true);
    } else {
        ui->pushButtonFraktalColor->setVisible(false);
    }
}


QColor MainWindow::getFraktalColor()
{
    if(ui->comboBoxMandelColor->currentText() == "<andere>") {
        return fraktalColor;
    } else {
        return QColor(ui->comboBoxMandelColor->currentText());
    }
}

void MainWindow::setFratalColor(int index, QColor eigen)
{
    if(index >= 0 && index < ui->comboBoxMandelColor->count()) {

        ui->comboBoxMandelColor->setCurrentIndex(index);

        if(ui->comboBoxMandelColor->currentText() == "<andere>") {
            ui->pushButtonFraktalColor->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + eigen.name() );
            fraktalColor = eigen;
            ui->pushButtonFraktalColor->setVisible(true);
        } else {
            ui->pushButtonFraktalColor->setVisible(false);
        }



    }
}



void MainWindow::on_comboBox_precession_currentIndexChanged(int)
{
    this->editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_doubleSpinBoxEscapeR_valueChanged(double value)
{
    this->editedSettings = true;
    this->setOperationMode();
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.setValue("ESCAPE_RADIUS", value );
}

#include <cmath>

void MainWindow::ZahlenFolge::setZahlenfolge(Point c_p, ImageSetting *s)
{
    if(this->isShown())
        this->removeZahlenfolge();

    double z_re, z_im, c_re, c_im;


    if ( s->isMandelbrotSet ) {
        z_re = 0;
        z_im = 0;
        c_re = s->mapImgPosReToGaus(c_p.x());
        c_im = s->mapImgPosImToGaus(c_p.y());

    } else {
        z_re = s->mapImgPosReToGaus(c_p.x());
        z_im = s->mapImgPosImToGaus(c_p.y());
        c_re = s->juliaStart_real;
        c_im = s->juliaStart_img;
    }

    qDebug() << "c_re" << c_re << " c_im" << c_im << " z_re " << z_re << " z_im " << z_im;
//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    QPoint addPoint;
        for(size_t i = 0; i < s->maxIterations &&i < 10000 && std::abs(addPoint.x()) < s->img_w && std::abs(addPoint.y()) < s->img_h; i++) {
            const double z_re2 = (z_re * z_re) - (z_im * z_im) + c_re;
            const double z_im2 = (2.0 * z_re * z_im) + c_im;
            addPoint = QPoint(s->mapGausReToImgPos(z_re2), s->mapGausImToImgPos(z_im2));
            this->zahlenfolge.push_back(addPoint);

            i++;
            z_re = (z_re2 * z_re2) - (z_im2 * z_im2) + c_re;
            z_im = (2.0 * z_re2 * z_im2) + c_im;
            addPoint = QPoint(s->mapGausReToImgPos(z_re), s->mapGausImToImgPos(z_im));
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
        this->updateRectPos(this->mousePos, this->mousePosInGauss);
    } else {
        this->rect.setSize(QSize(winW, (double)winH));
        this->updateRectPos(this->mousePos, this->mousePosInGauss);
    }
}

void MainWindow::ZoomRect::updateRectPos(QPoint pos, QPointF gauss)
{
    this->mousePos = pos;
    this->mousePosInGauss = gauss;
    auto left_top = pos - QPoint(rect.width() / 2, rect.height() / 2);
    this->rect.moveTopLeft(left_top);
    //    this->rect.setRect(left_top.x(), left_top.y(), rect.width(), rect.height());
}

QRect MainWindow::ZoomRect::getRect()
{
    return rect;
}

QPoint MainWindow::ZoomRect::getMousePos()
{
    return mousePos;
}

QPointF MainWindow::ZoomRect::getMausPosInGauss()
{
    return mousePosInGauss;
}

bool MainWindow::ZoomRect::isShown()
{
    return is_shown;
}

bool MainWindow::ZoomRect::rightMouseIsPressed()
{
    return right_mouse_press;
}






void MainWindow::on_spinBox_zoom_valueChanged(double)
{
    zoomRect.updateRectSize(this->ui->spinBoxW->value(), this->ui->spinBoxH->value(), ui->spinBox_zoom->value());
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
        s->cleanUP();
        delete s;
    }

    this->currentImg = settingsList.last();
    if(ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked())
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
    if(imgSerie.imgCount > 0)
        return;

    int id = item->data(187).toInt();
    for(int i = 0; i <  settingsList.length(); i++) {
        if(settingsList.at(i)->id == id) {

            this->currentImg = settingsList.at(i);
            if(ui->actionBilder_bei_Verlaufwechsel_neu_laden_2->isChecked())
                this->startRefresh(settingsList.at(i));
            else {
                this->endRefresh(false);
                updateImage();
            }
            ui->listWidgetHistory->setCurrentItem(item);


            if(ui->listWidgetHistory->row(item) < ui->listWidgetHistory->count() -1 )
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
    this->zoomRect.updateRectPos(QPoint(currentImg->mapGausReToImgPos(arg1), zoomRect.getMousePos().y()),
                                 QPointF(arg1, zoomRect.getMausPosInGauss().y()));
    this->zoomRect.show();
    editedSettings = true;
    this->setOperationMode();

    updateImage();
}


void MainWindow::on_im_valueChanged(double arg1)
{
    if(ignoreYPosEdited) {
        ignoreYPosEdited = false;
        return;
    }

    this->zoomRect.updateRectPos(QPoint(zoomRect.getMousePos().x(), currentImg->mapGausImToImgPos(-1.0* arg1)),
                                 QPointF(zoomRect.getMausPosInGauss().x(), -1.0* arg1));
    this->zoomRect.show();
    editedSettings = true;
    this->setOperationMode();

    updateImage();
}

#include <QClipboard>
#include <QGestureEvent>
#include <QIcon>
#include <QImageWriter>
#include <QImageWriter>
#include <QImageWriter>
#include <QSettings>
#include <QTemporaryFile>
#include <QTimer>

void MainWindow::pushButton_copy_clicked()
{
    QApplication::clipboard()->setImage(*this->currentImg->image);
    this->ui->statusbar->showMessage("Bild {Re(" + QString::number(this->currentImg->gaus_mid_re) + ") Im(" + QString::number(this->currentImg->gaus_mid_im) + ") 1:"
                                     +  QString::number(currentImg->scale()) + " [" + QString::number(currentImg->img_w) + "x" + QString::number(currentImg->img_h) + "]} in Zwischenablage kopiert." , 4000);
}


void MainWindow::on_pushButton_back_clicked()
{
    zustandWechseln("ZURÜCK");
}


void MainWindow::on_pushButtonHome_clicked()
{
    zustandWechseln("HOME");
}


void MainWindow::on_comboBox_Fraktal_currentIndexChanged(int)
{
    this->setOperationMode();
}

void MainWindow::on_spinBoxFarbWechselIntervalln_valueChanged(int)
{
    editedSettings = true;
    this->setOperationMode();
}




void MainWindow::on_spinBoxHSV_satursion_valueChanged(int)
{
    editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_spinBoxHSV_value_valueChanged(int)
{
    editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_spinBoxHSV_alpha_valueChanged(int)
{
    editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_UseLogButton_toggled(bool)
{
    editedSettings = true;
    this->setOperationMode();
}


void MainWindow::on_logFak_valueChanged(double)
{
    editedSettings = true;
    this->setOperationMode();
}


void MainWindow::pushColorButtonClicked(int button_num_array)
{

    //Bei QDarkStyle-Theme wird Maus gefangen!
    // int index = this->ui->comboBox_theme->currentIndex();
    // bool changed_theme = false;
    // if( QSysInfo::productType() != "macos" /*Qt 6*/ && QSysInfo::productType() != "macos"  /*Qt 5*/ ) {
    //     // changed_theme = true;
    //     /*BUG FIX:*/
    //     // if(index == 4)
    //     //     this->ui->comboBox_theme->setCurrentIndex(1);
    //     // else if(index == 5)
    //     //     this->ui->comboBox_theme->setCurrentIndex(3);

    // }

    auto color = QColorDialog::getColor(buttonColors[button_num_array], this, "Farbe " + QString::number(button_num_array + 1));
    // if(!color.isValid()) {
    //     if(changed_theme)
    //         this->ui->comboBox_theme->setCurrentIndex(index);
    //     return;
    // }

    buttonColors[button_num_array] = color;
    if(ui->comboBox_palette->currentText() == "<eigen>") {
        buttonColors_save[button_num_array] = color;
        this->updateIconInOwnColor(80);
        QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
        settingOwnColor.setValue("OWN_RGB_VERLAUF_OWN_COLOR_" + QString::number(button_num_array + 1), color.name() );
    }

    QPushButton * btn = nullptr;

    switch (button_num_array) {
    case 0:
        btn = ui->pushButton_color1;
        break;
    case 1:
        btn = ui->pushButton_color2;
        break;
    case 2:
        btn = ui->pushButton_color3;
        break;
    case 3:
        btn = ui->pushButton_color4;
        break;
    case 4:
        btn = ui->pushButton_color5;
        break;
    case 5:
        btn = ui->pushButton_color6;
        break;
    case 6:
        btn = ui->pushButton_color7;
        break;
    case 7:
        btn = ui->pushButton_color8;
        break;
    }
    if(btn)
        btn->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + buttonColors[button_num_array].toRgb().name() + ";" );
    editedSettings = true;
    this->setOperationMode();
    // if(changed_theme)
    //     this->ui->comboBox_theme->setCurrentIndex(index);
}


void MainWindow::on_pushButton_color1_clicked()
{
    pushColorButtonClicked(0);
}

void MainWindow::on_pushButton_color2_clicked()
{
    pushColorButtonClicked(1);

}

void MainWindow::on_pushButton_color3_clicked()
{
    pushColorButtonClicked(2);

}

void MainWindow::on_pushButton_color4_clicked()
{
    pushColorButtonClicked(3);

}

void MainWindow::on_pushButton_color5_clicked()
{
    pushColorButtonClicked(4);

}

void MainWindow::on_pushButton_color6_clicked()
{
    pushColorButtonClicked(5);

}

void MainWindow::on_pushButton_color7_clicked()
{
    pushColorButtonClicked(6);

}

void MainWindow::on_pushButton_color8_clicked()
{
    pushColorButtonClicked(7);

}




void MainWindow::on_pushButtonFraktalColor_clicked()
{
    //Bei QDarkStyle-Theme wird Maus gefangen!
    int index = this->ui->comboBox_theme->currentIndex();
    bool changed_theme = false;
    if( QSysInfo::productType() != "macos" /*Qt 6*/ && QSysInfo::productType() != "macos"  /*Qt 5*/ ) {
        changed_theme = true;
        /*BUG FIX:*/
        if(index == 4)
            this->ui->comboBox_theme->setCurrentIndex(1);
        else if(index == 5)
            this->ui->comboBox_theme->setCurrentIndex(3);

    }


    auto color = QColorDialog::getColor(fraktalColor, this, "Farbe 8");
    if(!color.isValid()) {
        if(changed_theme)
            this->ui->comboBox_theme->setCurrentIndex(index);
        return;
    }
    fraktalColor = color;

    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.setValue("OWN_FRAKTAL_COLOR", color.name() );

    ui->pushButtonFraktalColor->setStyleSheet(STYLE_SHEET_COLOR_BUTTON + fraktalColor.name() );
    editedSettings = true;
    this->setOperationMode();
    if(changed_theme)
        this->ui->comboBox_theme->setCurrentIndex(index);
}



void MainWindow::radioColorButtonToggeled(int button_num_array, bool status)
{
    editedSettings = true;
    this->setOperationMode();
    if(ui->comboBox_palette->currentText() == "<eigen>" && !dont_update_save_check_buttons) {
            isChecked_save[button_num_array] = status;
            QSettings("Markus@W-Sem_2022", "Fraktalgenerator").setValue("OWN_RGB_VERLAUF_OWN_CHECKED_" + QString::number(button_num_array + 1), status );
            this->updateIconInOwnColor(80);
    }
}

void MainWindow::on_radioButtonF1_toggled(bool status)
{
    radioColorButtonToggeled(0, status);
}


void MainWindow::on_radioButtonF2_toggled(bool status)
{
    radioColorButtonToggeled(1, status);
}

void MainWindow::on_radioButtonF3_toggled(bool status)
{
    radioColorButtonToggeled(2, status);
}

void MainWindow::on_radioButtonF4_toggled(bool status)
{
    radioColorButtonToggeled(3, status);
}

void MainWindow::on_radioButtonF5_toggled(bool status)
{
    radioColorButtonToggeled(4, status);
}

void MainWindow::on_radioButtonF6_toggled(bool status)
{
    radioColorButtonToggeled(5, status);
}


void MainWindow::on_radioButtonF7_toggled(bool status)
{
    radioColorButtonToggeled(6, status);
}


void MainWindow::on_radioButtonF8_toggled(bool status)
{
    radioColorButtonToggeled(7, status);
}

#include <QStyle>


void MainWindow::on_comboBox_theme_currentIndexChanged(int index)
{
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.setValue("THEME", index );

    QString fileName;
    QString data;

    // Reset the style of QApplication
    QApplication::setStyle(defaultStyle);
    this->setStyleSheet("");
    QApplication::setPalette(QPalette());
    ui->progressBar->setTextVisible(ui->comboBox_theme->currentIndex() != 0);

    qDebug() << "QStyleFactory: " << QStyleFactory::keys();

    if(index == 0) {

        //OLD
        //currentBackgroundColor = "white";

        //Qt 6+
        if(QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Light)
            currentBackgroundColor = "white";
        else
            currentBackgroundColor = QColor(60, 60, 60);

        return;

    } else  if(index == 1) {
        currentBackgroundColor = "white";
        QApplication::setStyle(QStyleFactory::create("Fusion"));

        return;

    }  else  if(index == 2) {
        currentBackgroundColor = "gray";

        // Set Fusion style
        QApplication::setStyle(QStyleFactory::create("Fusion"));

        //Customize color palette for dark mode
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));


        // Setzen Sie die Dark Palette für Ihre Anwendung
        QApplication::setPalette(darkPalette);
        this->setStyleSheet("QListWidget { background-color: #353535; color: #ffffff; }");

        return;

    } else  if(index == 3) {
        currentBackgroundColor = "QColor::fromRgb(25, 35 , 45 )";

        // Set Fusion style
        QApplication::setStyle(QStyleFactory::create("Fusion"));

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor("#1f1f2e"));            // Dunkelblau
        darkPalette.setColor(QPalette::WindowText, QColor("#f0f0f0"));        // Weiß
        darkPalette.setColor(QPalette::Base, QColor("#2e2e3e"));              // Dunkelgrau
        darkPalette.setColor(QPalette::AlternateBase, QColor("#1f1f2e"));     // Dunkelblau
        darkPalette.setColor(QPalette::ToolTipBase, QColor("#f0f0f0"));       // Weiß
        darkPalette.setColor(QPalette::ToolTipText, QColor("#f0f0f0"));       // Weiß
        darkPalette.setColor(QPalette::Text, QColor("#f0f0f0"));               // Weiß
        darkPalette.setColor(QPalette::Button, QColor("#2e2e3e"));             // Dunkelgrau
        darkPalette.setColor(QPalette::ButtonText, QColor("#f0f0f0"));         // Weiß
        darkPalette.setColor(QPalette::BrightText, QColor("#ff0000"));         // Hellrot
        darkPalette.setColor(QPalette::Link, QColor("#2a82da"));               // Blau

        QApplication::setPalette(darkPalette);
        this->setStyleSheet("QListWidget { background-color: #2e2e3e; color: #f0f0f0; }");

        return;

    } else {

        if( index == 4) {
            fileName = ":/qss/qss_icons/lightstyle.qss";
            currentBackgroundColor = "white";
        } else if( index == 5) {
            fileName = ":/qss/qss_icons/darkstyle.qss";
            currentBackgroundColor = QColor::fromRgb(25, 35 , 45 );
        } else {
            qInfo() << "Unknown Theme!";
            return;
        }

        //4 und 5:.................


        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug()<<"filenot opened";
            return;
        }
        else
        {
            qDebug()<<"file opened";
            data = file.readAll();

            /*data += ""
                "QGroupBox {"
                "    background-color: rgb(69, 83, 100);"
                "    margin-top:1em;"
                ""
                "}"
                "QLabel{"
                "    background-color: rgb(69, 83, 100);"
                ""
                "}"
                "QLabel:disabled{"
                "    background-color: rgb(69, 83, 100);"
                ""
                "}"
                ""
                "QGroupBox::title {"
                "    subcontrol-origin: padding;"
                "    subcontrol-position: left top;"
                "    background: transparent;"
                "    margin-top: -2.5em;"
                "}";*/
        }

        file.close();

        if( QSysInfo::productType() == "macos" /*Qt 6*/ || QSysInfo::productType() == "macos"  /*Qt 5*/  || true) {
            int index = data.indexOf("QComboBox::indicator {");
            if( index != -1 ) {
                int index2 = data.indexOf("}", index);
                if( index2 != -1 ) {
                    data.insert(index, "/*");
                    index2+= 2 + 1;
                    data.insert(index2, "*/");
                }
            }
        }


        this->setStyleSheet(data);

    }
}


void MainWindow::pushButtonShowFullScreen_clicked()
{
    if(this->fullScreenView->isVisible()) {
        this->setFullScreenWindowVisible(false);

    } else {
        this->setFullScreenWindowVisible(true);
    }
}

void MainWindow::on_spinBoxHauptScreen_valueChanged(int arg1)
{
    if( arg1 >= QApplication::screens().length() )
        return;
    QRect s = QApplication::screens().at(arg1)->geometry();
    fullScreenView->setGeometry(s);
    double scalefactor = QApplication::screens().at(arg1)->devicePixelRatio();
    ui->spinBoxW->setValue( s.width() * scalefactor);
    ui->spinBoxH->setValue( s.height() * scalefactor  );
    this->fullScreenView->setHdpiScale( scalefactor );
}

void MainWindow::pushButtonOpenImgSerieDialog_clicked()
{
    this->startImgFolge();
}


void MainWindow::pushButtonOpenVideoDialog_clicked()
{
    this->createVideo();
}


void MainWindow::on_actionBild_speichern_unter_triggered()
{
    this->pushButtonSaveImg_clicked();
}


void MainWindow::on_actionBild_in_Zwischenablage_kopieren_triggered()
{
    this->pushButton_copy_clicked();
}


void MainWindow::on_actionSeitenmen_anzeigen_toggled(bool arg1)
{
    ui->widget->setVisible(arg1);
}


void MainWindow::on_actionVollbildfenster_anzeigen_triggered(bool checked)
{
    this->setFullScreenWindowVisible(checked);
}


void MainWindow::on_actionBilderfolge_erstellen_triggered()
{
    this->startImgFolge();
}


void MainWindow::on_actionVideo_aus_Bilderfolge_generieren_triggered()
{
    this->createVideo();
}

#include "dialogueber.h"

void MainWindow::on_action_ber_triggered()
{
    DialogUeber duebr(P_VERSION, updater, this);
    duebr.exec();
//    QDesktopServices::openUrl(QUrl("https://github.com/M4RKUS28/Mandelbrot-Menge-Generator"));
}


void MainWindow::on_radioButtonKoords_2_clicked(bool)
{
    this->updateImage();
}


void MainWindow::on_actionEinstellungen_exportieren_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Einstellungen speichern in");
    if(path == "")
        return;
    if(currentImg->store_setting(path) == 0)
        ui->statusbar->showMessage("Speichere Einstellungen unter '" + path + "'...", 3000);
    else
        ui->statusbar->showMessage("Speicherung der Einstellungen unter '" + path + "' fehlgeschlagen!", 3000);

}


void MainWindow::on_actionEinstellungen_importien_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Einstellungsen importieren aus");
    if(path == "")
        return;
    qDebug() << path;
    this->loadImageFromSettings(path);

}

void MainWindow::loadImageFromSettings(QString path, bool isReadOnly)
{
    QTemporaryFile file;
    int bytes = 0;

    if(isReadOnly) {
        auto example = QFile(path);
        if(file.open() && example.open(QIODevice::ReadOnly)) {
            if(file.write(example.readAll()) == -1) {
                qDebug() << "COPY FAILED!";
                return;
            } else {
                bytes = file.size();
                if(bytes)
                    path = file.fileName();
                else
                    qDebug() << "COPY FAILED! Empyt file";
            }
        } else {
            qDebug() << "file.open() && example.open(QIODevice::ReadOnly) failed";
            return;
        }
    }



    ImageSetting * imgS = new ImageSetting((this->lastImgStructID = this->lastImgStructID + 1), path);
    if(imgS->loadFromFileWasOK) {
        this->updateUiWithImageSetting( imgS );

        this->op_mode = OP_MODE::LOAD_IMG_FROM_FILE;
        this->startRefresh(imgS, true);
        if(!isReadOnly)
            ui->statusbar->showMessage("Lade Einstellungen aus '" + path + "'...", 3000);
    } else {
        ui->statusbar->showMessage("Laden der Einstellungen aus '" + path + "' fehlgeschlagen!", 3000);
        delete imgS;
    }

}


void MainWindow::on_actionBeenden_triggered()
{
    QApplication::exit();
}


void MainWindow::on_comboBox_intervall_absolut_currentIndexChanged(int index)
{

    editedSettings = true;
    this->setOperationMode();

    if(index == 0) {
        ui->spinBoxFarbWechselIntervalln->show();
        ui->label_farbwechselintervalln->show();


    } else {
        ui->label_farbwechselintervalln->hide();
        ui->spinBoxFarbWechselIntervalln->hide();
    }
}


void MainWindow::resizeEvent(QResizeEvent *)
{
    if( ui->actionAutoscroll_in_Mitte->isChecked() ) {
        qDebug() << "Center..." << ui->scrollArea->verticalScrollBar()->maximum() / 2;
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum() / 2);
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->maximum() / 2);
    }
}

void MainWindow::on_spinBox_threads_valueChanged(int arg1)
{
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.setValue("THREAD_COUNT", arg1 );
}


void MainWindow::on_spinBoxStartVerzoegerung_valueChanged(int arg1)
{
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.setValue("THREAD_CREATE_TIME_DELAY", arg1 );
}


void MainWindow::on_actionStandart_Einstellungen_wiederherstellen_triggered()
{
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.clear();
    //restart
    QApplication::quit();
#ifdef Q_OS_WEB
    // Reload the current page to restart the application.
    EM_ASM(
        {
            location.reload();
        });
#else
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
#endif
}


void MainWindow::on_spinBoxZahlenfolgeLinienbreite_valueChanged(int)
{
    this->updateImage();
}


void MainWindow::on_spinBoxKoordPenThickness_valueChanged(int)
{
    this->updateImage();
}


void MainWindow::on_comboBoxColorKoordSystem_currentIndexChanged(int)
{
    this->updateImage();
}



void MainWindow::on_spinBoxBeschriftungen_valueChanged(double )
{
    this->updateImage();

}


void MainWindow::on_doubleSpinBox_Koord_x_start_valueChanged(double )
{
    this->updateImage();

}


void MainWindow::on_doubleSpinBox_Koord_x_end_valueChanged(double )
{
    this->updateImage();

}


void MainWindow::on_spinBoxBeschriftungenY_valueChanged(double )
{
    this->updateImage();

}


void MainWindow::on_doubleSpinBox_Koord_y_start_valueChanged(double )
{
    this->updateImage();

}


void MainWindow::on_doubleSpinBox_Koord_y_end_valueChanged(double )
{
    this->updateImage();

}


void MainWindow::on_comboBox_drawStyle_zahlenfolge_currentIndexChanged(int )
{
    this->updateImage();

}


void MainWindow::on_comboBox_color_zahlenfolge_currentIndexChanged(int )
{
    this->updateImage();

}

#include <QMessageBox>

void MainWindow::on_actionTastenk_rzel_triggered()
{
    QMessageBox::information(this, "Tastenkürzel",
                             "<html><head/><body><p><span style=\" font-weight:700; color:#0e71b3;\">Mausklicks</span><span style=\" font-style:italic; color:#0e71b3;\">------------------------------------------------------------------</span></p><p><span style=\" font-weight:700;\">-&gt; [Linksklick in Bild]:</span> Vorschau des neuen Bildes an Mausposition</p><p><span style=\" font-weight:700;\">-&gt; [Doppelklick in Bild]: </span>Berechnung an Mausposition starten</p><p><span style=\" font-weight:700;\">-&gt; [Rechtsclick in Bild]:</span> Zahlenfolge des Startwertes an der Mausposition</p><p><span style=\" font-weight:700; color:#16a358;\">Tastenkürze</span><span style=\" color:#16a358;\">l----------------------------------------------------------------</span></p><p><span style=\" color:#5b5b5b;\">---</span><span style=\" font-style:italic; color:#5b5b5b;\">Auswahl der Farbalgorithmen</span><span style=\" color:#5b5b5b;\">--------------------------------------------</span></p><p><span style=\" font-weight:700;\">-&gt; [1]...[9]</span>: RGB-Farbverläufe</p><p><span style=\" font-weight:700;\">-&gt; [9]...[10]: </span>HSV-Farbverläufe</p><p><span style=\" font-weight:700;\">-&gt; [X]...[Y]: </span>HSV-Farbverläufe</p><p><span style=\" font-weight:700;\">-&gt; [Z]: </span>Histogram-Farbverlauf</p><p><span style=\" font-style:italic; color:#5b5b5b;\">---Vollbildmodus------------------------------------------------------------</span></p><p><span style=\" font-weight:700;\">-&gt; [ESC], [Q], [E] oder [L]: </span>Vollbildfenster schließen</p><p><span style=\" font-style:italic; color:#5b5b5b;\">---Video erstellen</span><span style=\" font-style:italic; color:#5b5b5b;\">------------------------------------------------------------</span></p><p><span style=\" font-weight:700;\">-&gt; [O]: </span>Video aus Bilderfolge erstellen</p><p><span style=\" font-weight:700;\">-&gt; [I]: </span>Bilderfolge erstellen</p><p><span style=\" font-style:italic; color:#5b5b5b;\">---Berechnungseinstellungen</span><span style=\" font-style:italic; color:#5b5b5b;\">-----------------------------------------------</span></p><p><span style=\" font-weight:700;\">-&gt; [-]:</span> Zoomfaktor - 1</p><p><span style=\" font-weight:700;\">-&gt; [+]: </span>Zoomfaktor + 1</p><p><span style=\" font-weight:700;\">-&gt; [H]: </span>Iterationen - 1</p><p><span style=\" font-weight:700;\">-&gt; [T]: </span>Iterationen + 1</p><p><span style=\" font-style:italic; color:#5b5b5b;\">---Verlauf-------------------------------------------------------------------</span></p><p><span style=\" font-weight:700;\">-&gt; [N]: </span>Nächstes Bild im Verlauf</p><p><span style=\" font-weight:700;\">-&gt; [B]: </span>Vorheriges Bild im Verlauf</p></body></html>"
                             );
}


void MainWindow::on_actionBeispiel_1_triggered()
{
    this->loadImageFromSettings(":/examples/examples/example1", true);
}


void MainWindow::on_actionBespiel_2_triggered()
{
    this->loadImageFromSettings(":/examples/examples/example2", true);
}


void MainWindow::on_actionBeispiel_3_triggered()
{
    this->loadImageFromSettings(":/examples/examples/example3", true);
}


void MainWindow::on_actionBeispiel_4_triggered()
{
    this->loadImageFromSettings(":/examples/examples/example4", true);

}

void MainWindow::on_pushButtonResetUnappliedsettings_clicked()
{
    updateUiWithImageSetting(currentImg);
    this->editedSettings = false;
    this->setOperationMode();
}




void MainWindow::on_actionUnangewandte_Bildeinstellungen_zur_cksetzten_triggered()
{
    this->ui->pushButtonResetUnappliedsettings->click();
}




void MainWindow::on_listWidgetHistory_customContextMenuRequested(const QPoint &pos)
{
#ifdef Q_OS_WEB
    return;
#endif
    // wenn neben item geklickt...abbruch,
    auto item = ui->listWidgetHistory->itemAt(pos);
    if(!item )
        return;

    // Handle global position
    QPoint globalPos = ui->listWidgetHistory->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu(this);
    if(item->data(187).toInt() != currentImg->id) {
        myMenu.addAction("Laden",  this, SLOT(context_menue_history_load_clicked()))->setIcon(QIcon(QPixmap::fromImage(QImage(":/icons/icons/reload.png"))));

        myMenu.addSeparator();
    }
    myMenu.addAction("Umbennen", this, SLOT(context_menue_history_edit_clicked()))->setIcon(QIcon(QPixmap::fromImage(QImage(":/icons/icons/rename.png"))));
    //wenn gerade verwendetes, wenn item das erste ist, kein löschen...
    if(item->data(187).toInt() != ui->listWidgetHistory->item(0)->data(187).toInt() && item->data(187).toInt() != currentImg->id) {
        myMenu.addSeparator();
        myMenu.addAction("Löschen",  this, SLOT(context_menue_history_remove_clicked()))->setIcon(QIcon(QPixmap::fromImage(QImage(":/icons/icons/delete.png"))));
    }

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::context_menue_history_load_clicked()
{
    emit ui->listWidgetHistory->itemDoubleClicked(ui->listWidgetHistory->currentItem());
}




#include <QInputDialog>

void MainWindow::context_menue_history_edit_clicked()
{
    QListWidgetItem * item;
    bool ok;
    QString newName;

    if((item = ui->listWidgetHistory->currentItem()))
        if (!(newName = QInputDialog::getText(nullptr, "Listenname ändern", "Neuer Name:", QLineEdit::Normal, item->text(), &ok)).isEmpty() && ok)
            item->setText(newName);
}

void MainWindow::context_menue_history_remove_clicked()
{
    auto item = ui->listWidgetHistory->currentItem();
    if(!item)
        return;

    //wenn item das erste ist, abbruch...
    if( ui->listWidgetHistory->count() < 1 || item == ui->listWidgetHistory->item(0) || item->data(187).toInt() == currentImg->id)
        return;

    int id = item->data(187).toInt();
    for(int i = 0; i <  settingsList.length(); i++) {
        if(settingsList.at(i)->id == id) {
            auto s = settingsList.takeAt(i);
            s->cleanUP();
            delete s;
            auto it = ui->listWidgetHistory->takeItem(ui->listWidgetHistory->row(item));
            if(it)
                delete it;
            if(ui->listWidgetHistory->count() > 0 && currentImg->id == ui->listWidgetHistory->item(ui->listWidgetHistory->count() - 1)->data(187).toInt())
                ui->pushButton_vor->setDisabled(true);

        }
    }
}

void MainWindow::on_groupBox_2_toggled(bool)
{
    updateImage();
}


void MainWindow::on_listWidgetHistory_itemClicked(QListWidgetItem *)
{

}




void MainWindow::on_actionNach_Updates_suchen_triggered(bool checked)
{
    updater->setAutoSearchForUpdate(checked);
    ui->actionNach_Updates_suchen->setChecked(updater->getAutoSearchForUpdateStatus());
}

void MainWindow::pinchRecieved(bool zoomIn, QPoint posCenter, QPoint posCenterScaled)
{
    ui->spinBox_zoom->setValue((zoomIn ? 1 : -1) * std::abs(ui->spinBox_zoom->value()));

    QMouseEvent* doubleClickEvent = new QMouseEvent(
        QEvent::MouseButtonDblClick,
        posCenter,
        Qt::LeftButton,
        Qt::NoButton,
        Qt::NoModifier
    );

    zustandWechseln("DOPPELKLICK_IN_BILD", "", posCenterScaled, doubleClickEvent);
}

