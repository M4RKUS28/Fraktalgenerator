#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "workerthread.h"

#include <QLinearGradient>

#include "imageview.h"
#include "math.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#define START_SCALE 120
#define START_POS_X -80
#define START_POS_Y 0

struct Point{
    Point();
    Point(ssize_t X, ssize_t Y);

public:
    ssize_t y() const;
    void setY(ssize_t newY);
    ssize_t x() const;
    void setX(ssize_t newX);

    QPoint rountToQPoint();

private:
    ssize_t X;
    ssize_t Y;

};

struct ImageSetting {
    ImageSetting(long double scale, Point midPoint );
    void init(size_t img_w, size_t img_h, size_t maxIterations);
    void cleanUP();
    void setIterationCountAt(ssize_t x, ssize_t y, size_t iterations);

    size_t getIterationCountAt(QPoint pos);
    size_t getIterationCountAt(ssize_t x, ssize_t y);

    long double PixelToImag(QPoint p);
    long double PixelToReal(QPoint p);

    QImage * image;
    QPainter * painter;
    Point midPoint;

    long double scale;
    long double logEscape;

    ssize_t x_verschiebung;
    ssize_t y_verschiebung;
    ssize_t img_w;
    ssize_t img_h;
    ssize_t maxIterations;

    QLine xAchse;
    QLine yAchse;

private:
    size_t **iterations;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct ZoomRect{
        ZoomRect();
        void show();
        void hide();
        void updateRectSize(size_t winW, size_t winH, long double scale);
        void updateRectPos(Point p);
        void setMousePressState(bool isPressed);

        QRect getRect();
        Point getMousePos();
        bool isShown();
        bool rightMouseIsPressed();


    private:
        Point mousePos;
        QRect rect;
        bool is_shown = false;
        bool right_mouse_press = false;

    };

    struct ZahlenFolge {
        void setZahlenfolge(Point c_p, ImageSetting s);
        void removeZahlenfolge();

        QList<QPoint> getZahlenfolge();
        bool isShown();

    private:
        QList<QPoint> zahlenfolge;
        bool show = false;

    };

    enum OP_MODE {
        ZOOM_TO_SELECTED,
        REFRESH,
        APPLY_SETTINGS,
        APPLY_SETTINGS_AND_ZOOM,
        RESET
    };


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Liste für Zurück-Funktion
    QList<ImageSetting> settingsList;
    // Liste mit Threads für Berechnung
    QList<class WorkerThread* > tworkers;
    // Für Normalized Iteration Count Berechnung
    static constexpr long double log2 = 0.69314718055994530941723212145818;//0.30102999566398119521373889472449;
    // für größenvorschau && speicherung der letzten Mausposition
    ZoomRect zoomRect;
    // timer only update img if there are changes
    bool imgUpdateNeeded = false;
    // Operation mode -> reload, zoom, applay settings etc.
    OP_MODE op_mode;
    // Für anzeige der benötigten Iterationen
    ZahlenFolge zahlenfolge;
    //Timer für TimerEvent
    int timerID;
    // derzeitiges Bild
    ImageSetting currentImg;

private:
    enum STATE {
        STOPED,
        RUNNING
    } state = STATE::STOPED;

public:
    ImageSetting getNewScaledSetting(ImageSetting last_img);

    void updateUiWithImageSetting(ImageSetting imgs);

    void startRefresh(ImageSetting set, bool appendToList = false);

    void stopThreads();

    void endRefresh();

    void setOperationMode(OP_MODE o);

    void setColor(QPainter *mpainter, size_t iters, std::complex<long double> z_n);

    void timerEvent(QTimerEvent *);

    bool checkForFinished();

    void afterColoring(ImageSetting set);


    void zustandWechseln(QString aktion, QString wert_s = "", QPoint p = QPoint(), QMouseEvent * keyInput = nullptr);

    void updateImage();







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

    void on_spinBox_zoom_valueChanged(double arg1);

    void mouse_move_in_img(QPoint pos);

    void on_comboBox_currentIndexChanged(int index);

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
