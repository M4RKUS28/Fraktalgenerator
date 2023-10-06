#ifndef DIALOGUEBER_H
#define DIALOGUEBER_H

#include <QDialog>
#include <QDebug>
#include "updater.h"


namespace Ui {
class DialogUeber;
}

class DialogUeber : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUeber(QString PROGRAMM_VERSION, Updater * updater, QWidget *parent = nullptr);
    ~DialogUeber();

    Updater * updater;
    void updateUpdaterButtonAndTexts();

private slots:
    void on_pushButtonStartUpdate_clicked();
    void on_pushButtonCheckForUpdates_clicked();
    void updaterStatusChanged();

private:
    Ui::DialogUeber *ui;
    QString PROGRAMM_VERSION;
};

#endif // DIALOGUEBER_H
