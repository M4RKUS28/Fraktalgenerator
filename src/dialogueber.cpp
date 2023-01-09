#include "dialogueber.h"
#include "ui_dialogueber.h"

DialogUeber::DialogUeber(QString PROGRAMM_VERSION, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUeber),
    PROGRAMM_VERSION(PROGRAMM_VERSION)
{
    ui->setupUi(this);
    ui->label_version->setText("Fraktalgenerator " + PROGRAMM_VERSION);
    ui->label_qt_version->setText("Qt " + QString(QT_VERSION_STR));
    ui->label_compile_date->setText(QString(__DATE__) + QString(" at ") + QString(__TIME__));
    ui->label_arch->setText(QSysInfo::currentCpuArchitecture());

    if( QSysInfo::productType() == "macos" /*Qt 6*/ || QSysInfo::productType() == "macos"  /*Qt 5*/ ) {





    }

}

DialogUeber::~DialogUeber()
{
    qDebug() << "~DialogUeber()";

    delete ui;
}
