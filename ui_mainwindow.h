/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSpeichern_unter;
    QAction *actionBild_in_Zwischenablage_kopieren;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QProgressBar *progressBar;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QFormLayout *formLayout_2;
    QLabel *label_6;
    QSpinBox *spinBoxIterations;
    QLabel *label_9;
    QComboBox *comboBox_palette;
    QFrame *line_13;
    QRadioButton *radioButton_normalized;
    QRadioButton *radioButton_invert;
    QRadioButton *radioButton;
    QFrame *line_12;
    QLabel *label_12;
    QFrame *line_2;
    QLabel *label_8;
    QLabel *label_2;
    QLabel *re;
    QLabel *label_13;
    QLabel *img;
    QLabel *label_14;
    QLabel *label_iterations;
    QLabel *lineEditScaleAbs;
    QLabel *label_4;
    QSpinBox *spinBox_zoom;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *bmRe;
    QLabel *bmIm;
    QWidget *tab_2;
    QFormLayout *formLayout;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBoxW;
    QLabel *label_11;
    QSpinBox *spinBoxH;
    QLabel *label;
    QComboBox *comboBox_background_color;
    QGroupBox *groupBoxMandelFarbe;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBoxMandelColor;
    QFrame *line_3;
    QLabel *label_5;
    QSpinBox *spinBox_threads;
    QLabel *label_10;
    QComboBox *comboBox_precession;
    QLabel *label_15;
    QDoubleSpinBox *doubleSpinBoxEscapeR;
    QRadioButton *radioButton_reload_at_back;
    QSpacerItem *verticalSpacer_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelFraktal;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pushButtonStart;
    QFrame *frameButtons;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_2;
    QPushButton *pushButtonSaveImg;
    QPushButton *pushButton;
    QFrame *line;
    QLabel *label_7;
    QMenuBar *menubar;
    QMenu *menuSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1002, 653);
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        MainWindow->setMouseTracking(false);
        actionSpeichern_unter = new QAction(MainWindow);
        actionSpeichern_unter->setObjectName(QString::fromUtf8("actionSpeichern_unter"));
        actionBild_in_Zwischenablage_kopieren = new QAction(MainWindow);
        actionBild_in_Zwischenablage_kopieren->setObjectName(QString::fromUtf8("actionBild_in_Zwischenablage_kopieren"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy);
        progressBar->setMinimumSize(QSize(300, 15));
        progressBar->setValue(0);
        progressBar->setInvertedAppearance(false);

        gridLayout->addWidget(progressBar, 3, 0, 1, 1);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(200, 400));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(0, 200));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        formLayout_2 = new QFormLayout(tab);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        label_6->setFont(font1);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_6);

        spinBoxIterations = new QSpinBox(tab);
        spinBoxIterations->setObjectName(QString::fromUtf8("spinBoxIterations"));
        spinBoxIterations->setMinimum(0);
        spinBoxIterations->setMaximum(999999);
        spinBoxIterations->setValue(1000);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, spinBoxIterations);

        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font1);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_9);

        comboBox_palette = new QComboBox(tab);
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->setObjectName(QString::fromUtf8("comboBox_palette"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, comboBox_palette);

        line_13 = new QFrame(tab);
        line_13->setObjectName(QString::fromUtf8("line_13"));
        sizePolicy1.setHeightForWidth(line_13->sizePolicy().hasHeightForWidth());
        line_13->setSizePolicy(sizePolicy1);
        line_13->setMinimumSize(QSize(0, 0));
        line_13->setFrameShape(QFrame::HLine);
        line_13->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(4, QFormLayout::SpanningRole, line_13);

        radioButton_normalized = new QRadioButton(tab);
        radioButton_normalized->setObjectName(QString::fromUtf8("radioButton_normalized"));
        radioButton_normalized->setChecked(true);
        radioButton_normalized->setAutoExclusive(false);

        formLayout_2->setWidget(5, QFormLayout::LabelRole, radioButton_normalized);

        radioButton_invert = new QRadioButton(tab);
        radioButton_invert->setObjectName(QString::fromUtf8("radioButton_invert"));
        radioButton_invert->setAutoExclusive(false);

        formLayout_2->setWidget(6, QFormLayout::LabelRole, radioButton_invert);

        radioButton = new QRadioButton(tab);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setAutoExclusive(false);

        formLayout_2->setWidget(7, QFormLayout::LabelRole, radioButton);

        line_12 = new QFrame(tab);
        line_12->setObjectName(QString::fromUtf8("line_12"));
        line_12->setFrameShape(QFrame::HLine);
        line_12->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(9, QFormLayout::SpanningRole, line_12);

        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        label_12->setFont(font2);

        formLayout_2->setWidget(12, QFormLayout::LabelRole, label_12);

        line_2 = new QFrame(tab);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(13, QFormLayout::SpanningRole, line_2);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font1);

        formLayout_2->setWidget(14, QFormLayout::LabelRole, label_8);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setFont(font2);
        label_2->setAlignment(Qt::AlignCenter);

        formLayout_2->setWidget(18, QFormLayout::LabelRole, label_2);

        re = new QLabel(tab);
        re->setObjectName(QString::fromUtf8("re"));

        formLayout_2->setWidget(18, QFormLayout::FieldRole, re);

        label_13 = new QLabel(tab);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font2);
        label_13->setAlignment(Qt::AlignCenter);

        formLayout_2->setWidget(19, QFormLayout::LabelRole, label_13);

        img = new QLabel(tab);
        img->setObjectName(QString::fromUtf8("img"));

        formLayout_2->setWidget(19, QFormLayout::FieldRole, img);

        label_14 = new QLabel(tab);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font2);

        formLayout_2->setWidget(20, QFormLayout::LabelRole, label_14);

        label_iterations = new QLabel(tab);
        label_iterations->setObjectName(QString::fromUtf8("label_iterations"));

        formLayout_2->setWidget(20, QFormLayout::FieldRole, label_iterations);

        lineEditScaleAbs = new QLabel(tab);
        lineEditScaleAbs->setObjectName(QString::fromUtf8("lineEditScaleAbs"));

        formLayout_2->setWidget(12, QFormLayout::FieldRole, lineEditScaleAbs);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        formLayout_2->setWidget(10, QFormLayout::LabelRole, label_4);

        spinBox_zoom = new QSpinBox(tab);
        spinBox_zoom->setObjectName(QString::fromUtf8("spinBox_zoom"));
        spinBox_zoom->setMinimum(1);
        spinBox_zoom->setMaximum(999999);
        spinBox_zoom->setValue(2);

        formLayout_2->setWidget(10, QFormLayout::FieldRole, spinBox_zoom);

        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignCenter);

        formLayout_2->setWidget(15, QFormLayout::LabelRole, label_16);

        label_17 = new QLabel(tab);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignCenter);

        formLayout_2->setWidget(16, QFormLayout::LabelRole, label_17);

        label_18 = new QLabel(tab);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font1);

        formLayout_2->setWidget(17, QFormLayout::LabelRole, label_18);

        bmRe = new QLabel(tab);
        bmRe->setObjectName(QString::fromUtf8("bmRe"));

        formLayout_2->setWidget(15, QFormLayout::FieldRole, bmRe);

        bmIm = new QLabel(tab);
        bmIm->setObjectName(QString::fromUtf8("bmIm"));

        formLayout_2->setWidget(16, QFormLayout::FieldRole, bmIm);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        formLayout = new QFormLayout(tab_2);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spinBoxW = new QSpinBox(tab_2);
        spinBoxW->setObjectName(QString::fromUtf8("spinBoxW"));
        spinBoxW->setMaximum(9999);
        spinBoxW->setValue(600);

        horizontalLayout->addWidget(spinBoxW);

        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_11);

        spinBoxH = new QSpinBox(tab_2);
        spinBoxH->setObjectName(QString::fromUtf8("spinBoxH"));
        spinBoxH->setMaximum(9999);
        spinBoxH->setValue(400);

        horizontalLayout->addWidget(spinBoxH);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout);

        label = new QLabel(tab_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font1);

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        comboBox_background_color = new QComboBox(tab_2);
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->addItem(QString());
        comboBox_background_color->setObjectName(QString::fromUtf8("comboBox_background_color"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBox_background_color);

        groupBoxMandelFarbe = new QGroupBox(tab_2);
        groupBoxMandelFarbe->setObjectName(QString::fromUtf8("groupBoxMandelFarbe"));
        groupBoxMandelFarbe->setCheckable(true);
        groupBoxMandelFarbe->setChecked(false);
        horizontalLayout_2 = new QHBoxLayout(groupBoxMandelFarbe);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        comboBoxMandelColor = new QComboBox(groupBoxMandelFarbe);
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->addItem(QString());
        comboBoxMandelColor->setObjectName(QString::fromUtf8("comboBoxMandelColor"));

        horizontalLayout_2->addWidget(comboBoxMandelColor);


        formLayout->setWidget(2, QFormLayout::SpanningRole, groupBoxMandelFarbe);

        line_3 = new QFrame(tab_2);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        formLayout->setWidget(3, QFormLayout::SpanningRole, line_3);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font2);

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        spinBox_threads = new QSpinBox(tab_2);
        spinBox_threads->setObjectName(QString::fromUtf8("spinBox_threads"));
        spinBox_threads->setMinimum(1);
        spinBox_threads->setMaximum(999);
        spinBox_threads->setValue(20);

        formLayout->setWidget(4, QFormLayout::FieldRole, spinBox_threads);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font2);

        formLayout->setWidget(5, QFormLayout::LabelRole, label_10);

        comboBox_precession = new QComboBox(tab_2);
        comboBox_precession->addItem(QString());
        comboBox_precession->addItem(QString());
        comboBox_precession->setObjectName(QString::fromUtf8("comboBox_precession"));
        sizePolicy1.setHeightForWidth(comboBox_precession->sizePolicy().hasHeightForWidth());
        comboBox_precession->setSizePolicy(sizePolicy1);
        comboBox_precession->setMinimumSize(QSize(0, 0));
        comboBox_precession->setFont(font);

        formLayout->setWidget(5, QFormLayout::FieldRole, comboBox_precession);

        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_15);

        doubleSpinBoxEscapeR = new QDoubleSpinBox(tab_2);
        doubleSpinBoxEscapeR->setObjectName(QString::fromUtf8("doubleSpinBoxEscapeR"));
        doubleSpinBoxEscapeR->setMaximum(9999.989999999999782);
        doubleSpinBoxEscapeR->setValue(4.000000000000000);

        formLayout->setWidget(6, QFormLayout::FieldRole, doubleSpinBoxEscapeR);

        radioButton_reload_at_back = new QRadioButton(tab_2);
        radioButton_reload_at_back->setObjectName(QString::fromUtf8("radioButton_reload_at_back"));
        radioButton_reload_at_back->setChecked(false);

        formLayout->setWidget(7, QFormLayout::SpanningRole, radioButton_reload_at_back);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        gridLayout->addWidget(widget, 0, 1, 1, 1);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 684, 564));
        horizontalLayout_5 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        labelFraktal = new QLabel(scrollAreaWidgetContents_2);
        labelFraktal->setObjectName(QString::fromUtf8("labelFraktal"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelFraktal->sizePolicy().hasHeightForWidth());
        labelFraktal->setSizePolicy(sizePolicy3);
        labelFraktal->setMaximumSize(QSize(16777215, 16777215));
        labelFraktal->setMouseTracking(true);
        labelFraktal->setFrameShape(QFrame::StyledPanel);
        labelFraktal->setFrameShadow(QFrame::Plain);
        labelFraktal->setScaledContents(false);
        labelFraktal->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_5->addWidget(labelFraktal);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea, 0, 0, 2, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(20, 0, 20, 0);
        pushButtonStart = new QPushButton(frame_2);
        pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));

        verticalLayout_4->addWidget(pushButtonStart);


        verticalLayout_2->addWidget(frame_2);

        frameButtons = new QFrame(centralwidget);
        frameButtons->setObjectName(QString::fromUtf8("frameButtons"));
        frameButtons->setFrameShape(QFrame::StyledPanel);
        frameButtons->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frameButtons);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(20, 0, 20, -1);
        pushButton_2 = new QPushButton(frameButtons);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);

        verticalLayout_3->addWidget(pushButton_2);

        pushButtonSaveImg = new QPushButton(frameButtons);
        pushButtonSaveImg->setObjectName(QString::fromUtf8("pushButtonSaveImg"));
        pushButtonSaveImg->setEnabled(false);

        verticalLayout_3->addWidget(pushButtonSaveImg);

        pushButton = new QPushButton(frameButtons);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_3->addWidget(pushButton);


        verticalLayout_2->addWidget(frameButtons);

        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);


        gridLayout->addLayout(verticalLayout_2, 1, 1, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 3, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1002, 23));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSettings->menuAction());
        menuSettings->addAction(actionSpeichern_unter);
        menuSettings->addAction(actionBild_in_Zwischenablage_kopieren);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSpeichern_unter->setText(QCoreApplication::translate("MainWindow", "Bild Speichern unter", nullptr));
#if QT_CONFIG(shortcut)
        actionSpeichern_unter->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBild_in_Zwischenablage_kopieren->setText(QCoreApplication::translate("MainWindow", "Bild in Zwischenablage kopieren", nullptr));
#if QT_CONFIG(shortcut)
        actionBild_in_Zwischenablage_kopieren->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        label_6->setText(QCoreApplication::translate("MainWindow", "Iterationen", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Farbpalette:", nullptr));
        comboBox_palette->setItemText(0, QCoreApplication::translate("MainWindow", "Alpha Wert", nullptr));
        comboBox_palette->setItemText(1, QCoreApplication::translate("MainWindow", "KA RGB", nullptr));
        comboBox_palette->setItemText(2, QCoreApplication::translate("MainWindow", "SchwarzWeis", nullptr));

        radioButton_normalized->setText(QCoreApplication::translate("MainWindow", "Normalized", nullptr));
        radioButton_invert->setText(QCoreApplication::translate("MainWindow", "Farben Invertieren", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Koordinaten Achsen", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Absolute Vergr\303\266\303\237erung:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Bildmittelpunkt:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "     Re.:", nullptr));
        re->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "     Im.:", nullptr));
        img->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "     Iterationen:", nullptr));
        label_iterations->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lineEditScaleAbs->setText(QCoreApplication::translate("MainWindow", "1:200", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Vergr\303\266\303\237erungsfaktor:", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "     Re.:", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "     Im.:", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Mausposition:", nullptr));
        bmRe->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        bmIm->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\303\234bersicht", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Aufl\303\266sung:", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Hintergrundfarbe:", nullptr));
        comboBox_background_color->setItemText(0, QCoreApplication::translate("MainWindow", "cyan", nullptr));
        comboBox_background_color->setItemText(1, QCoreApplication::translate("MainWindow", "blue", nullptr));
        comboBox_background_color->setItemText(2, QCoreApplication::translate("MainWindow", "red", nullptr));
        comboBox_background_color->setItemText(3, QCoreApplication::translate("MainWindow", "yellow", nullptr));
        comboBox_background_color->setItemText(4, QCoreApplication::translate("MainWindow", "orange", nullptr));
        comboBox_background_color->setItemText(5, QCoreApplication::translate("MainWindow", "green", nullptr));
        comboBox_background_color->setItemText(6, QCoreApplication::translate("MainWindow", "pink", nullptr));
        comboBox_background_color->setItemText(7, QCoreApplication::translate("MainWindow", "black", nullptr));
        comboBox_background_color->setItemText(8, QCoreApplication::translate("MainWindow", "white", nullptr));

        groupBoxMandelFarbe->setTitle(QCoreApplication::translate("MainWindow", "Mandelbrot-Menge Farbe:", nullptr));
        comboBoxMandelColor->setItemText(0, QCoreApplication::translate("MainWindow", "black", nullptr));
        comboBoxMandelColor->setItemText(1, QCoreApplication::translate("MainWindow", "white", nullptr));
        comboBoxMandelColor->setItemText(2, QCoreApplication::translate("MainWindow", "cyan", nullptr));
        comboBoxMandelColor->setItemText(3, QCoreApplication::translate("MainWindow", "yellow", nullptr));
        comboBoxMandelColor->setItemText(4, QCoreApplication::translate("MainWindow", "pink", nullptr));
        comboBoxMandelColor->setItemText(5, QCoreApplication::translate("MainWindow", "brown", nullptr));
        comboBoxMandelColor->setItemText(6, QCoreApplication::translate("MainWindow", "green", nullptr));
        comboBoxMandelColor->setItemText(7, QCoreApplication::translate("MainWindow", "blue", nullptr));
        comboBoxMandelColor->setItemText(8, QCoreApplication::translate("MainWindow", "orange", nullptr));
        comboBoxMandelColor->setItemText(9, QCoreApplication::translate("MainWindow", "red", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow", "Threads:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Genauigkeit:", nullptr));
        comboBox_precession->setItemText(0, QCoreApplication::translate("MainWindow", "double<8 Bit>", nullptr));
        comboBox_precession->setItemText(1, QCoreApplication::translate("MainWindow", "double<16 Bit>", nullptr));

        label_15->setText(QCoreApplication::translate("MainWindow", "Fluchradius:", nullptr));
        radioButton_reload_at_back->setText(QCoreApplication::translate("MainWindow", "Lade Bilder bei Zur\303\274ck neu", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Weitere Einstellungen", nullptr));
        labelFraktal->setText(QCoreApplication::translate("MainWindow", "Image", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "Vergr\303\266\303\237ern", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Zur\303\274ck", nullptr));
        pushButtonSaveImg->setText(QCoreApplication::translate("MainWindow", "Bild Speichern", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Home", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Made by Markus", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Datei", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
