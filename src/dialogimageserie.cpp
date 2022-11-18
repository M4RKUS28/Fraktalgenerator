#include "dialogimageserie.h"
#include "ui_dialogimageserie.h"

#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QString>

DialogImageSerie::DialogImageSerie(QWidget *parent, ImgSerie s) :
    QDialog(parent),
    ui(new Ui::DialogImageSerie)
{
    ui->setupUi(this);
    this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(true);

    this->dir = s.dirPath;
    this->ui->lineEditPath->setText(dir);

    this->ui->lineEditNamePrefix->setText( s.prefix );
    this->ui->spinBox_NameItStart->setValue( s.nameStartConst );
    this->ui->spinBoxBilderZahl->setValue( s.imgCountConst );
    this->ui->doubleSpinBoxZoomStep->setValue( s.zoomStep );
    this->ui->spinBoxFPS->setValue( s.fps );

}


DialogImageSerie::~DialogImageSerie()
{
    delete ui;
}

DialogImageSerie::ImgSerie DialogImageSerie::getImgSerieSettings()
{
    return ImgSerie(this->ui->doubleSpinBoxZoomStep->value(), ui->spinBoxBilderZahl->value(), dir, ui->spinBox_NameItStart->value(), ui->lineEditNamePrefix->text(), ui->spinBoxFPS->value());
}

void DialogImageSerie::on_pushButtonOpenSaveDir_clicked()
{
    QString dir_ = QFileDialog::getExistingDirectory( this, tr("Ordner auswählen"), this->dir, QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks );
    if(!dir_.isEmpty() && QDir(dir_).exists()) {
        this->ui->lineEditPath->setText( this->dir = dir_ );
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(false);
    } else {
        this->ui->lineEditPath->setText( "" );
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(true);
    }
}


DialogImageSerie::ImgSerie::ImgSerie()
{
    this->zoomStep = 1.01;
    this->dirPath = "";

    //wichtig 0 == deaktiviert
    this->imgCount = 0;
    this->imgCountConst = 200;

    this->nameItStart = 0;
    this->nameStartConst = nameItStart;

    this->prefix = "";
    this->fps = 30;
}

DialogImageSerie::ImgSerie::ImgSerie(double zoom, unsigned imgCount, QString path, unsigned nameItStart, QString prefix, unsigned fps)
{
    this->zoomStep = zoom;
    this->imgCount = imgCount;
    this->imgCountConst = imgCount;

    this->dirPath = path;
    this->nameItStart = nameItStart;
    this->nameStartConst = nameItStart;

    this->prefix = prefix;

    this->fps = fps;

}





void DialogImageSerie::on_lineEditPath_textChanged(const QString &arg1)
{
    if(QDir(arg1).exists()) {
        this->dir = arg1;
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(false);
    } else {
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(true);
    }
}

