/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
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
#include "Imageview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer;
    QLabel *label_19;
    QPushButton *pushButton_scale_plus;
    QPushButton *pushButton_scale_minus;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_7;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QFormLayout *formLayout_5;
    QFrame *line_4;
    QLabel *label_23;
    QLabel *label_12;
    QLabel *lineEditScaleAbs;
    QLabel *label_8;
    QLabel *label_16;
    QLabel *bmRe;
    QLabel *label_17;
    QLabel *bmIm;
    QLabel *label_18;
    QLabel *label_2;
    QLabel *re;
    QLabel *label_13;
    QLabel *img;
    QLabel *label_14;
    QLabel *label_iterations;
    QFrame *line_5;
    QFrame *line_6;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QFrame *line_8;
    QGroupBox *groupBoxMandelFarbe;
    QFormLayout *formLayout;
    QComboBox *comboBoxMandelColor;
    QLabel *label_22;
    QGroupBox *groupBox;
    QFormLayout *formLayout_3;
    QLabel *label_9;
    QComboBox *comboBox_palette;
    QLabel *label;
    QComboBox *comboBox_background_color;
    QFrame *line_2;
    QRadioButton *radioButton_normalized;
    QRadioButton *radioButton_invert;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_4;
    QLabel *label_20;
    QComboBox *comboBox_color_zahlenfolge;
    QLabel *label_21;
    QComboBox *comboBox_drawStyle_zahlenfolge;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_6;
    QLabel *label_24;
    QComboBox *comboBoxColorKoordSystem;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab;
    QFormLayout *formLayout_2;
    QFrame *line_7;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBoxW;
    QLabel *label_11;
    QSpinBox *spinBoxH;
    QLabel *label_6;
    QSpinBox *spinBoxMaxIterations;
    QLabel *label_4;
    QDoubleSpinBox *spinBox_zoom;
    QFrame *line_3;
    QRadioButton *radioButtonKoords;
    QLabel *label_25;
    QSpinBox *spinBoxBeschriftungen;
    QFrame *line_9;
    QRadioButton *radioButton_zahelnfolge_bei_mousemove;
    QRadioButton *radioButton_reload_at_back;
    QFrame *line_12;
    QLabel *label_5;
    QSpinBox *spinBox_threads;
    QLabel *label_15;
    QDoubleSpinBox *doubleSpinBoxEscapeR;
    QLabel *label_10;
    QComboBox *comboBox_precession;
    QSpacerItem *verticalSpacer;
    QRadioButton *radioButtonMitBeschriftungen;
    QLabel *label_26;
    QSpinBox *spinBoxBeschriftungenY;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_5;
    ImageView *imageView;
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
    QMenuBar *menubar;
    QMenu *menuSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 726);
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        MainWindow->setMouseTracking(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
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

        horizontalLayout_3->addWidget(progressBar);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        label_19 = new QLabel(centralwidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setEnabled(false);

        horizontalLayout_3->addWidget(label_19);

        pushButton_scale_plus = new QPushButton(centralwidget);
        pushButton_scale_plus->setObjectName(QString::fromUtf8("pushButton_scale_plus"));
        pushButton_scale_plus->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton_scale_plus->sizePolicy().hasHeightForWidth());
        pushButton_scale_plus->setSizePolicy(sizePolicy);
        pushButton_scale_plus->setMinimumSize(QSize(35, 35));
        pushButton_scale_plus->setMaximumSize(QSize(35, 35));

        horizontalLayout_3->addWidget(pushButton_scale_plus);

        pushButton_scale_minus = new QPushButton(centralwidget);
        pushButton_scale_minus->setObjectName(QString::fromUtf8("pushButton_scale_minus"));
        pushButton_scale_minus->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton_scale_minus->sizePolicy().hasHeightForWidth());
        pushButton_scale_minus->setSizePolicy(sizePolicy);
        pushButton_scale_minus->setMinimumSize(QSize(35, 35));
        pushButton_scale_minus->setMaximumSize(QSize(35, 35));

        horizontalLayout_3->addWidget(pushButton_scale_minus);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 4, 1, 1, 1);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(200, 400));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(280, 425));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        formLayout_5 = new QFormLayout(tab_3);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        line_4 = new QFrame(tab_3);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        formLayout_5->setWidget(0, QFormLayout::SpanningRole, line_4);

        label_23 = new QLabel(tab_3);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        label_23->setFont(font1);

        formLayout_5->setWidget(1, QFormLayout::SpanningRole, label_23);

        label_12 = new QLabel(tab_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        label_12->setFont(font2);

        formLayout_5->setWidget(2, QFormLayout::LabelRole, label_12);

        lineEditScaleAbs = new QLabel(tab_3);
        lineEditScaleAbs->setObjectName(QString::fromUtf8("lineEditScaleAbs"));

        formLayout_5->setWidget(2, QFormLayout::FieldRole, lineEditScaleAbs);

        label_8 = new QLabel(tab_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font1);

        formLayout_5->setWidget(4, QFormLayout::SpanningRole, label_8);

        label_16 = new QLabel(tab_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignCenter);

        formLayout_5->setWidget(5, QFormLayout::LabelRole, label_16);

        bmRe = new QLabel(tab_3);
        bmRe->setObjectName(QString::fromUtf8("bmRe"));

        formLayout_5->setWidget(5, QFormLayout::FieldRole, bmRe);

        label_17 = new QLabel(tab_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignCenter);

        formLayout_5->setWidget(6, QFormLayout::LabelRole, label_17);

        bmIm = new QLabel(tab_3);
        bmIm->setObjectName(QString::fromUtf8("bmIm"));

        formLayout_5->setWidget(6, QFormLayout::FieldRole, bmIm);

        label_18 = new QLabel(tab_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font1);

        formLayout_5->setWidget(8, QFormLayout::SpanningRole, label_18);

        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setFont(font2);
        label_2->setAlignment(Qt::AlignCenter);

        formLayout_5->setWidget(9, QFormLayout::LabelRole, label_2);

        re = new QLabel(tab_3);
        re->setObjectName(QString::fromUtf8("re"));

        formLayout_5->setWidget(9, QFormLayout::FieldRole, re);

        label_13 = new QLabel(tab_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font2);
        label_13->setAlignment(Qt::AlignCenter);

        formLayout_5->setWidget(10, QFormLayout::LabelRole, label_13);

        img = new QLabel(tab_3);
        img->setObjectName(QString::fromUtf8("img"));

        formLayout_5->setWidget(10, QFormLayout::FieldRole, img);

        label_14 = new QLabel(tab_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font2);

        formLayout_5->setWidget(11, QFormLayout::LabelRole, label_14);

        label_iterations = new QLabel(tab_3);
        label_iterations->setObjectName(QString::fromUtf8("label_iterations"));

        formLayout_5->setWidget(11, QFormLayout::FieldRole, label_iterations);

        line_5 = new QFrame(tab_3);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        formLayout_5->setWidget(3, QFormLayout::SpanningRole, line_5);

        line_6 = new QFrame(tab_3);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        formLayout_5->setWidget(7, QFormLayout::SpanningRole, line_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout_5->setItem(12, QFormLayout::SpanningRole, verticalSpacer_2);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        line_8 = new QFrame(tab_2);
        line_8->setObjectName(QString::fromUtf8("line_8"));
        line_8->setFrameShape(QFrame::HLine);
        line_8->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line_8);

        groupBoxMandelFarbe = new QGroupBox(tab_2);
        groupBoxMandelFarbe->setObjectName(QString::fromUtf8("groupBoxMandelFarbe"));
        groupBoxMandelFarbe->setFont(font1);
        groupBoxMandelFarbe->setCheckable(true);
        groupBoxMandelFarbe->setChecked(false);
        formLayout = new QFormLayout(groupBoxMandelFarbe);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(25, 1, -1, 1);
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
        comboBoxMandelColor->setFont(font2);

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBoxMandelColor);

        label_22 = new QLabel(groupBoxMandelFarbe);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setFont(font2);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_22);


        verticalLayout_5->addWidget(groupBoxMandelFarbe);

        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font1);
        formLayout_3 = new QFormLayout(groupBox);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setContentsMargins(25, 1, 9, 1);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font1);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_9);

        comboBox_palette = new QComboBox(groupBox);
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->addItem(QString());
        comboBox_palette->setObjectName(QString::fromUtf8("comboBox_palette"));
        comboBox_palette->setFont(font2);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, comboBox_palette);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font2);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label);

        comboBox_background_color = new QComboBox(groupBox);
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
        comboBox_background_color->setFont(font2);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, comboBox_background_color);

        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        formLayout_3->setWidget(2, QFormLayout::SpanningRole, line_2);

        radioButton_normalized = new QRadioButton(groupBox);
        radioButton_normalized->setObjectName(QString::fromUtf8("radioButton_normalized"));
        radioButton_normalized->setFont(font1);
        radioButton_normalized->setChecked(true);
        radioButton_normalized->setAutoExclusive(false);

        formLayout_3->setWidget(3, QFormLayout::SpanningRole, radioButton_normalized);

        radioButton_invert = new QRadioButton(groupBox);
        radioButton_invert->setObjectName(QString::fromUtf8("radioButton_invert"));
        radioButton_invert->setFont(font2);
        radioButton_invert->setAutoExclusive(false);

        formLayout_3->setWidget(4, QFormLayout::SpanningRole, radioButton_invert);


        verticalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font1);
        formLayout_4 = new QFormLayout(groupBox_2);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setHorizontalSpacing(6);
        formLayout_4->setContentsMargins(25, 1, -1, 1);
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font2);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_20);

        comboBox_color_zahlenfolge = new QComboBox(groupBox_2);
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->addItem(QString());
        comboBox_color_zahlenfolge->setObjectName(QString::fromUtf8("comboBox_color_zahlenfolge"));
        comboBox_color_zahlenfolge->setFont(font2);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, comboBox_color_zahlenfolge);

        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_21);

        comboBox_drawStyle_zahlenfolge = new QComboBox(groupBox_2);
        comboBox_drawStyle_zahlenfolge->addItem(QString());
        comboBox_drawStyle_zahlenfolge->addItem(QString());
        comboBox_drawStyle_zahlenfolge->addItem(QString());
        comboBox_drawStyle_zahlenfolge->setObjectName(QString::fromUtf8("comboBox_drawStyle_zahlenfolge"));
        comboBox_drawStyle_zahlenfolge->setFont(font2);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, comboBox_drawStyle_zahlenfolge);


        verticalLayout_5->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setFont(font1);
        formLayout_6 = new QFormLayout(groupBox_3);
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        formLayout_6->setContentsMargins(25, 1, -1, 4);
        label_24 = new QLabel(groupBox_3);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setFont(font2);

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_24);

        comboBoxColorKoordSystem = new QComboBox(groupBox_3);
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->addItem(QString());
        comboBoxColorKoordSystem->setObjectName(QString::fromUtf8("comboBoxColorKoordSystem"));
        comboBoxColorKoordSystem->setFont(font2);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, comboBoxColorKoordSystem);


        verticalLayout_5->addWidget(groupBox_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        formLayout_2 = new QFormLayout(tab);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        line_7 = new QFrame(tab);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(0, QFormLayout::SpanningRole, line_7);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spinBoxW = new QSpinBox(tab);
        spinBoxW->setObjectName(QString::fromUtf8("spinBoxW"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(spinBoxW->sizePolicy().hasHeightForWidth());
        spinBoxW->setSizePolicy(sizePolicy3);
        spinBoxW->setMaximum(9999);
        spinBoxW->setValue(1200);

        horizontalLayout->addWidget(spinBoxW);

        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_11);

        spinBoxH = new QSpinBox(tab);
        spinBoxH->setObjectName(QString::fromUtf8("spinBoxH"));
        spinBoxH->setMaximum(9999);
        spinBoxH->setValue(720);

        horizontalLayout->addWidget(spinBoxH);


        formLayout_2->setLayout(1, QFormLayout::FieldRole, horizontalLayout);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font1);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        spinBoxMaxIterations = new QSpinBox(tab);
        spinBoxMaxIterations->setObjectName(QString::fromUtf8("spinBoxMaxIterations"));
        spinBoxMaxIterations->setMinimum(0);
        spinBoxMaxIterations->setMaximum(999999);
        spinBoxMaxIterations->setValue(1000);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, spinBoxMaxIterations);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_4);

        spinBox_zoom = new QDoubleSpinBox(tab);
        spinBox_zoom->setObjectName(QString::fromUtf8("spinBox_zoom"));
        spinBox_zoom->setDecimals(1);
        spinBox_zoom->setMinimum(-99999.000000000000000);
        spinBox_zoom->setMaximum(99999.000000000000000);
        spinBox_zoom->setValue(2.000000000000000);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, spinBox_zoom);

        line_3 = new QFrame(tab);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(4, QFormLayout::SpanningRole, line_3);

        radioButtonKoords = new QRadioButton(tab);
        radioButtonKoords->setObjectName(QString::fromUtf8("radioButtonKoords"));
        radioButtonKoords->setAutoExclusive(false);

        formLayout_2->setWidget(5, QFormLayout::SpanningRole, radioButtonKoords);

        label_25 = new QLabel(tab);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        formLayout_2->setWidget(7, QFormLayout::LabelRole, label_25);

        spinBoxBeschriftungen = new QSpinBox(tab);
        spinBoxBeschriftungen->setObjectName(QString::fromUtf8("spinBoxBeschriftungen"));
        spinBoxBeschriftungen->setValue(16);

        formLayout_2->setWidget(7, QFormLayout::FieldRole, spinBoxBeschriftungen);

        line_9 = new QFrame(tab);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setFrameShape(QFrame::HLine);
        line_9->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(9, QFormLayout::SpanningRole, line_9);

        radioButton_zahelnfolge_bei_mousemove = new QRadioButton(tab);
        radioButton_zahelnfolge_bei_mousemove->setObjectName(QString::fromUtf8("radioButton_zahelnfolge_bei_mousemove"));
        radioButton_zahelnfolge_bei_mousemove->setChecked(true);
        radioButton_zahelnfolge_bei_mousemove->setAutoExclusive(false);

        formLayout_2->setWidget(10, QFormLayout::SpanningRole, radioButton_zahelnfolge_bei_mousemove);

        radioButton_reload_at_back = new QRadioButton(tab);
        radioButton_reload_at_back->setObjectName(QString::fromUtf8("radioButton_reload_at_back"));
        radioButton_reload_at_back->setChecked(false);
        radioButton_reload_at_back->setAutoExclusive(false);

        formLayout_2->setWidget(11, QFormLayout::SpanningRole, radioButton_reload_at_back);

        line_12 = new QFrame(tab);
        line_12->setObjectName(QString::fromUtf8("line_12"));
        line_12->setFrameShape(QFrame::HLine);
        line_12->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(12, QFormLayout::SpanningRole, line_12);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font2);

        formLayout_2->setWidget(13, QFormLayout::LabelRole, label_5);

        spinBox_threads = new QSpinBox(tab);
        spinBox_threads->setObjectName(QString::fromUtf8("spinBox_threads"));
        spinBox_threads->setMinimum(1);
        spinBox_threads->setMaximum(999);
        spinBox_threads->setValue(20);

        formLayout_2->setWidget(13, QFormLayout::FieldRole, spinBox_threads);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout_2->setWidget(14, QFormLayout::LabelRole, label_15);

        doubleSpinBoxEscapeR = new QDoubleSpinBox(tab);
        doubleSpinBoxEscapeR->setObjectName(QString::fromUtf8("doubleSpinBoxEscapeR"));
        doubleSpinBoxEscapeR->setMinimum(2.000000000000000);
        doubleSpinBoxEscapeR->setMaximum(9999.989999999999782);
        doubleSpinBoxEscapeR->setValue(8.000000000000000);

        formLayout_2->setWidget(14, QFormLayout::FieldRole, doubleSpinBoxEscapeR);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font2);

        formLayout_2->setWidget(15, QFormLayout::LabelRole, label_10);

        comboBox_precession = new QComboBox(tab);
        comboBox_precession->addItem(QString());
        comboBox_precession->addItem(QString());
        comboBox_precession->setObjectName(QString::fromUtf8("comboBox_precession"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(comboBox_precession->sizePolicy().hasHeightForWidth());
        comboBox_precession->setSizePolicy(sizePolicy4);
        comboBox_precession->setMinimumSize(QSize(0, 0));
        comboBox_precession->setFont(font);

        formLayout_2->setWidget(15, QFormLayout::FieldRole, comboBox_precession);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout_2->setItem(16, QFormLayout::SpanningRole, verticalSpacer);

        radioButtonMitBeschriftungen = new QRadioButton(tab);
        radioButtonMitBeschriftungen->setObjectName(QString::fromUtf8("radioButtonMitBeschriftungen"));
        radioButtonMitBeschriftungen->setChecked(true);
        radioButtonMitBeschriftungen->setAutoExclusive(false);

        formLayout_2->setWidget(6, QFormLayout::SpanningRole, radioButtonMitBeschriftungen);

        label_26 = new QLabel(tab);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        formLayout_2->setWidget(8, QFormLayout::LabelRole, label_26);

        spinBoxBeschriftungenY = new QSpinBox(tab);
        spinBoxBeschriftungenY->setObjectName(QString::fromUtf8("spinBoxBeschriftungenY"));
        spinBoxBeschriftungenY->setValue(10);

        formLayout_2->setWidget(8, QFormLayout::FieldRole, spinBoxBeschriftungenY);

        tabWidget->addTab(tab, QString());

        verticalLayout->addWidget(tabWidget);


        gridLayout->addWidget(widget, 0, 1, 1, 1);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 676, 593));
        horizontalLayout_5 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        imageView = new ImageView(scrollAreaWidgetContents_2);
        imageView->setObjectName(QString::fromUtf8("imageView"));

        horizontalLayout_5->addWidget(imageView);

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

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 24));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSettings->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Skalierung:", nullptr));
        pushButton_scale_plus->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        pushButton_scale_minus->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Made by Markus --> mit Linksclick zahelnfolge!", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Aktuelles Bild:", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "     Vergr\303\266\303\237erung:", nullptr));
        lineEditScaleAbs->setText(QCoreApplication::translate("MainWindow", "1:200", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Bildmittelpunkt:", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "     Re.:", nullptr));
        bmRe->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "     Im.:", nullptr));
        bmIm->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Mausposition:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "     Re.:", nullptr));
        re->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "     Im.:", nullptr));
        img->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "     Iterationen:", nullptr));
        label_iterations->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "\342\255\220\303\234bersicht", nullptr));
        groupBoxMandelFarbe->setTitle(QCoreApplication::translate("MainWindow", "Mandelbrot-Menge", nullptr));
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

        label_22->setText(QCoreApplication::translate("MainWindow", "Farbe:", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Flucht-Menge:", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Farbpalette:", nullptr));
        comboBox_palette->setItemText(0, QCoreApplication::translate("MainWindow", "Alpha Wert", nullptr));
        comboBox_palette->setItemText(1, QCoreApplication::translate("MainWindow", "KA RGB", nullptr));
        comboBox_palette->setItemText(2, QCoreApplication::translate("MainWindow", "SchwarzWeis", nullptr));
        comboBox_palette->setItemText(3, QCoreApplication::translate("MainWindow", "Alpha2", nullptr));
        comboBox_palette->setItemText(4, QCoreApplication::translate("MainWindow", "HSV", nullptr));
        comboBox_palette->setItemText(5, QCoreApplication::translate("MainWindow", "HSL", nullptr));

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

        radioButton_normalized->setText(QCoreApplication::translate("MainWindow", "Normalized", nullptr));
        radioButton_invert->setText(QCoreApplication::translate("MainWindow", "Farben Invertieren", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Zahlenfolge:", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Farbe:", nullptr));
        comboBox_color_zahlenfolge->setItemText(0, QCoreApplication::translate("MainWindow", "white", nullptr));
        comboBox_color_zahlenfolge->setItemText(1, QCoreApplication::translate("MainWindow", "back", nullptr));
        comboBox_color_zahlenfolge->setItemText(2, QCoreApplication::translate("MainWindow", "cyan", nullptr));
        comboBox_color_zahlenfolge->setItemText(3, QCoreApplication::translate("MainWindow", "red", nullptr));
        comboBox_color_zahlenfolge->setItemText(4, QCoreApplication::translate("MainWindow", "blue", nullptr));
        comboBox_color_zahlenfolge->setItemText(5, QCoreApplication::translate("MainWindow", "yellow", nullptr));
        comboBox_color_zahlenfolge->setItemText(6, QCoreApplication::translate("MainWindow", "pink", nullptr));
        comboBox_color_zahlenfolge->setItemText(7, QCoreApplication::translate("MainWindow", "green", nullptr));
        comboBox_color_zahlenfolge->setItemText(8, QCoreApplication::translate("MainWindow", "orange", nullptr));

        label_21->setText(QCoreApplication::translate("MainWindow", "Zeichenstil:", nullptr));
        comboBox_drawStyle_zahlenfolge->setItemText(0, QCoreApplication::translate("MainWindow", "Punkte", nullptr));
        comboBox_drawStyle_zahlenfolge->setItemText(1, QCoreApplication::translate("MainWindow", "Verbundene Linien", nullptr));
        comboBox_drawStyle_zahlenfolge->setItemText(2, QCoreApplication::translate("MainWindow", "Linien mit Kreise", nullptr));

        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Koordinatensystem:", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Farbe:", nullptr));
        comboBoxColorKoordSystem->setItemText(0, QCoreApplication::translate("MainWindow", "white", nullptr));
        comboBoxColorKoordSystem->setItemText(1, QCoreApplication::translate("MainWindow", "black", nullptr));
        comboBoxColorKoordSystem->setItemText(2, QCoreApplication::translate("MainWindow", "gray", nullptr));
        comboBoxColorKoordSystem->setItemText(3, QCoreApplication::translate("MainWindow", "yellow", nullptr));
        comboBoxColorKoordSystem->setItemText(4, QCoreApplication::translate("MainWindow", "blue", nullptr));
        comboBoxColorKoordSystem->setItemText(5, QCoreApplication::translate("MainWindow", "pink", nullptr));
        comboBoxColorKoordSystem->setItemText(6, QCoreApplication::translate("MainWindow", "brown", nullptr));
        comboBoxColorKoordSystem->setItemText(7, QCoreApplication::translate("MainWindow", "cyan", nullptr));
        comboBoxColorKoordSystem->setItemText(8, QCoreApplication::translate("MainWindow", "green", nullptr));
        comboBoxColorKoordSystem->setItemText(9, QCoreApplication::translate("MainWindow", "red", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\360\237\216\250Farbe", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Aufl\303\266sung:", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Iterationen:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Zoomfaktor:", nullptr));
        spinBox_zoom->setPrefix(QCoreApplication::translate("MainWindow", " x ", nullptr));
        radioButtonKoords->setText(QCoreApplication::translate("MainWindow", "Koordinatensystem anzeigen", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Beschriftungen X:", nullptr));
        radioButton_zahelnfolge_bei_mousemove->setText(QCoreApplication::translate("MainWindow", "Zahlenfolge bei Bausbewegung", nullptr));
        radioButton_reload_at_back->setText(QCoreApplication::translate("MainWindow", "Lade Bilder bei Zur\303\274ck neu", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Threads:", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Fluchradius:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Genauigkeit:", nullptr));
        comboBox_precession->setItemText(0, QCoreApplication::translate("MainWindow", "double<8 Bit>", nullptr));
        comboBox_precession->setItemText(1, QCoreApplication::translate("MainWindow", "double<16 Bit>", nullptr));

        radioButtonMitBeschriftungen->setText(QCoreApplication::translate("MainWindow", "Mit Skalierung", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Beschriftungen Y:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\342\232\231Settings", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "Vergr\303\266\303\237ern", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Zur\303\274ck", nullptr));
        pushButtonSaveImg->setText(QCoreApplication::translate("MainWindow", "Bild Speichern", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Home", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
