#ifndef DIALOGCREATEVIDEO_H
#define DIALOGCREATEVIDEO_H

#include <QDialog>
#include <QSize>

#include "dialogimageserie.h"
#include "QAviWriter.h"



namespace Ui {
class DialogCreateVideo;
}

class DialogCreateVideo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreateVideo(QWidget *parent = nullptr, DialogImageSerie::ImgSerie imgS = DialogImageSerie::ImgSerie() );
    ~DialogCreateVideo();

    QString getImgsPath();
    QString getPrefix();
    QString getSuffix();

    int getNameItStart();
    int getNameItEnd();

    int getFPS();

    QString getWaveAudio();

    QString getFilePath();

    QString getCodec();


private slots:
    void on_pushButtonOpenSaveDir_clicked();

    void on_lineEditPath_textChanged(const QString &arg1);

    void on_lineEditFilePath_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::DialogCreateVideo *ui;
};

#endif // DIALOGCREATEVIDEO_H
