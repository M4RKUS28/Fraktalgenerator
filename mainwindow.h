#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "workerthread.h"

#include <QLinearGradient>

#include "imageview.h"

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

    struct Point{
        Point();
        Point(long long X, long long Y);

        long long X;
        long long Y;


    public:
        long long y() const;
        void setY(long long newY);
        long long x() const;
        void setX(long long newX);

    };

    struct SETTING {
        SETTING(long double scale, Point midPoint );
        void init(size_t img_w, size_t img_h, size_t maxIterations);
        void setIterationCountAt(size_t x, size_t y, size_t iterations);
        size_t getIterationCountAt(QPoint pos);
        size_t getIterationCountAt(size_t x, size_t y);

        long double PixelToImag(QPoint p);
        long double PixelToReal(QPoint p);


        QImage * image;
        QPainter * painter;
        long double scale;
        ssize_t x_verschiebung,
            y_verschiebung,
            img_w,
            img_h;
        Point midPoint;
        size_t maxIterations;

        QLine xAchse, yAchse;
    private:
        size_t **iterations;

        //alle anderen auch!!!!!
    };

    QList<SETTING> settings;


    QList<class WorkerThread* > tworkers;
//    QList<QList<WorkerThread::Pixel>> writequeque;

    long double logEscape,
                log2;

    struct MousePress{
        MousePress();
        void setPressed();
        void updateKoords(Point p, size_t x_left_corner, size_t add_y);
        Point point,
            koord;
        //for moving / in paintevent
        bool isPressed = false;
    } mousePress;

    // timer only update img if there are changes
    bool imgUpdateNeeded = false;

    enum OP_MODE {
        ZOOM_TO_SELECTED,
        REFRESH,
        APPLY_SETTINGS,
        APPLY_SETTINGS_AND_ZOOM,
        RESET
    } op_mode;


    struct ZahlenFolge {
        void setZahlenfolge(Point c_p, SETTING s);
        void removeZahlenfolge();

        QList<QPoint> getZahlenfolge();
        bool isShown();

    private:
        QList<QPoint> zahlenfolge;
        bool show = false;

    } zahlenfolge;

    ImageView * i;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



    void timerEvent(QTimerEvent *);

    void zustandWechseln(QString aktion, QString wert_s = "", size_t wert_i = 0, QMouseEvent * keyInput = nullptr);

    SETTING getNewScaledSetting(SETTING last);
    long double getNewScale(SETTING lastSet);

    void stopThreads();

    void setColor(QPainter *mpainter, size_t iters, std::complex<long double> z_n);

    void startRefresh(SETTING set, bool zoom = false);

    void endRefresh();

    void loadInitImg();

    void updateImage();

    bool checkForFinished();

    void afterColoring(SETTING set);

    void setOperationMode(OP_MODE o);




private slots:
    void on_pushButtonStart_clicked();

    void finishedLine(QList<WorkerThread::Pixel> *);

    void threadFinished();

    void on_pushButtonSaveImg_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


    void on_spinBoxMaxIterations_valueChanged(int arg1);

    void on_comboBox_palette_currentIndexChanged(int index);

    void on_radioButton_normalized_toggled(bool checked);

    void on_radioButton_invert_toggled(bool checked);

    void on_radioButton_toggled(bool checked);

    void on_spinBox_zoom_valueChanged(int arg1);

    void on_spinBoxW_valueChanged(int arg1);

    void on_spinBoxH_valueChanged(int arg1);

    void on_comboBox_background_color_currentIndexChanged(int index);

    void on_groupBoxMandelFarbe_toggled(bool arg1);

    void on_comboBoxMandelColor_currentIndexChanged(int index);

    void on_comboBox_precession_currentIndexChanged(int index);

    void on_doubleSpinBoxEscapeR_valueChanged(double arg1);

    void on_radioButton_reload_at_back_toggled(bool checked);

    void on_pushButton_scale_plus_clicked();

    void on_pushButton_scale_minus_clicked();

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
