#include "dialogueber.h"
#include "ui_dialogueber.h"

#include "updater.h"

#include <QThread>
#include <QTime>

DialogUeber::DialogUeber(QString PROGRAMM_VERSION, Updater * updater, QWidget *parent) :
    QDialog(parent),
    updater(updater),
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
    updater->setAutoShowMessageBox(false);
    updateUpdaterButtonAndTexts();
    connect(updater, SIGNAL(statusChanged()), this, SLOT(updaterStatusChanged()));
}

DialogUeber::~DialogUeber()
{
    qDebug() << "~DialogUeber()";
    disconnect(updater, SIGNAL(statusChanged()), this, SLOT(updaterStatusChanged()));
    delete ui;
}

void DialogUeber::updaterStatusChanged()
{
    updateUpdaterButtonAndTexts();
}

void DialogUeber::delay(int sec)
{
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QApplication::processEvents(QEventLoop::AllEvents, 100);
}


void DialogUeber::updateUpdaterButtonAndTexts()
{
    ui->pushButtonStartUpdate->hide();
    ui->pushButtonCheckForUpdates->hide();
    ui->label_7->setText(updater->getStatusStr());

    switch (updater->getStatus()) {
    case Updater::NOT_CHECKED:
        ui->pushButtonCheckForUpdates->show();
        break;
    case Updater::UPDTAE_NEEDED:
        ui->pushButtonStartUpdate->show();
        break;
    case Updater::UP_TO_DATE:
        ui->label_7->setText("Sie verwenden die neuste Version! Erneut nach Updates suchen?");
        ui->pushButtonCheckForUpdates->show();
        break;
    case Updater::UPDATING: {
        ui->label_7->setText("Updater ausgeführt! Programm schließt sich in 3 Sekunden!");
        this->delay();
        ui->label_7->setText("Updater ausgeführt! Programm schließt sich in 2 Sekunden!");
        this->delay();
        ui->label_7->setText("Updater ausgeführt! Programm schließt sich in 1 Sekunden!");
        this->delay(2);
        QApplication::exit();
        break;
        }
    default:
        break;
    }
}


void DialogUeber::on_pushButtonCheckForUpdates_clicked()
{
    updater->checkForUpdates();
}

void DialogUeber::on_pushButtonStartUpdate_clicked()
{
    updater->startUpdate();
}

