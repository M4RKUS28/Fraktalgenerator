#ifndef DIALOGUEBER_H
#define DIALOGUEBER_H

#include <QDialog>

namespace Ui {
class DialogUeber;
}

class DialogUeber : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUeber(QString PROGRAMM_VERSION, QWidget *parent = nullptr);
    ~DialogUeber();

private:
    Ui::DialogUeber *ui;
    QString PROGRAMM_VERSION;
};

#endif // DIALOGUEBER_H
