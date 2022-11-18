#ifndef DIALOGIMAGESERIE_H
#define DIALOGIMAGESERIE_H

#include <QDialog>

namespace Ui {
class DialogImageSerie;
}

class DialogImageSerie : public QDialog
{
    Q_OBJECT
public:

    struct ImgSerie{
        ImgSerie();
        ImgSerie(double zoom, unsigned imgCount, QString path, unsigned nameItStart, QString prefix, unsigned fps);
        double zoomStep;
        unsigned imgCount,
            imgCountConst;
        QString dirPath;
        unsigned nameItStart,
            nameStartConst;
        unsigned fps;
        QString prefix;
    };

    explicit DialogImageSerie(QWidget *parent = nullptr, ImgSerie s = ImgSerie());
    ~DialogImageSerie();


    ImgSerie getImgSerieSettings();

private slots:
    void on_pushButtonOpenSaveDir_clicked();

    void on_lineEditPath_textChanged(const QString &arg1);

private:
    Ui::DialogImageSerie *ui;
    QString dir;
};

#endif // DIALOGIMAGESERIE_H
