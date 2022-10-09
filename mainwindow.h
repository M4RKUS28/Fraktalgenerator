#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "workerthread.h"

#include <QLinearGradient>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum STATE {
        INIT,
        STOPED,
        RUNNING

    } state = STATE::INIT;


public:
    struct SETTING {
        SETTING(long double scale, QPoint midPoint );
        void init(int img_w, int img_h);

        QImage * image;
        QPainter * painter;
        int **iterations;
        long double scale;
        int x_verschiebung,
            y_verschiebung,
            img_w,
            img_h;
        QPoint midPoint;
    };

    QList<SETTING> settings;


    QList<class WorkerThread* > tworkers;
//    QList<QList<WorkerThread::Pixel>> writequeque;

    long double logEscape,
                log2;

    struct MousePress{
        void setPress(QPoint p, int x_left_corner, int add_y) {
            point = p;
            koord = p + QPoint(x_left_corner, add_y);
            isPressed = true;
        }
        QPoint point,
            koord;
        //for moving / in paintevent
        bool isPressed = false;
    } mousePress;

    // timer only update img if there are changes
    bool imgUpdateNeeded = false;
    QString zoomOrReload = "RELOAD";



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void timerEvent(QTimerEvent *);

    void zustandWechseln(QString aktion, QString wert_s = "", int wert_i = 0, QMouseEvent * keyInput = nullptr);

    SETTING getNewScaledSetting(SETTING last);

    void stopThreads();

    void setColor(QPainter *mpainter, size_t iters, std::complex<long double> z_n);

    void startRefresh(SETTING set, bool zoom = false);

    void endRefresh();

    void loadInitImg();

    void updateImage();

    bool checkForFinished();

private slots:
    void on_pushButtonStart_clicked();

    void finishedLine(QList<WorkerThread::Pixel> *);

    void threadFinished();

    void on_pushButtonSaveImg_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);


    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);


};








#endif // MAINWINDOW_H
