#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "workerthread.h"
#include<complex>


#include <QLinearGradient>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QColorDialog>
#include <QMap>


#include "imageview.h"
#include "math.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#define START_SCALE 220
#define START_POS_X -0.75
#define START_POS_Y 0.0

#define START_POS_X_JUL 0.0
#define START_POS_Y_JUL 0.0

const QString STYLE_SHEET_COLOR_BUTTON = "border: 1px solid black; border-radius: 3px; background-color: ";



class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct ZoomRect{
        ZoomRect();
        void show();
        void hide();
        void updateRectSize(size_t winW, size_t winH, double scale);
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
        void setZahlenfolge(Point c_p, ImageSetting *s);
        void removeZahlenfolge();

        QList<QPoint> getZahlenfolge();
        bool isShown();

    private:
        QList<QPoint> zahlenfolge;
        bool show = false;

    };

    enum OP_MODE {
        REFRESH,

        APPLY_SETTINGS,
        ZOOM_TO_SELECTED,
        APPLY_SETTINGS_AND_ZOOM,

        CALC_JULIA,
        CALC_JULIA_SETTINGS,

        BACK_TO_MANDELBROT,
    };


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Liste für Zurück-Funktion
    QList<ImageSetting*> settingsList;
    // Liste mit Threads für Berechnung
    QList<class WorkerThread* > tworkers;

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
    ImageSetting *currentImg;
    //id counter für liste und listwidget mapping
    size_t lastImgStructID;
    //für buttonmodus
    bool editedSettings;
    //lade nicht einstellungen ( bei zurück doer Home )
    bool noUpdateGui;
    // bei vor zurück oder home setzte settingsedited auf true
    bool isBackOrVor;
    // damit setText niucht als eigene Eingabe für Startwert gewertet wird
    bool ignoreXPosEdited, ignoreYPosEdited;
    void updateMidPos(bool clear = false);
    //buttonColors
    QColor buttonColors[5];

    QMap<int, bool> keyPressed;

    ImageView * fullScreenView;

private:
    enum STATE {
        STOPED,
        RUNNING
    } state = STATE::STOPED;

public:
    ImageSetting *getNewScaledSetting(ImageSetting *last_img);

    void updateUiWithImageSetting(ImageSetting *imgs);

    void startRefresh(ImageSetting *set, bool appendToList = false);

    void stopThreads();

    void endRefresh(bool appendToListHistory );

    void setOperationMode();


//    void setColor(QPainter *mpainter, size_t iters, std::complex<double> z_n);

    void timerEvent(QTimerEvent *);

    bool checkForFinished();

    void afterColoring(ImageSetting *set);


    void zustandWechseln(QString aktion, QString wert_s = "", QPoint p = QPoint(), QMouseEvent * keyInput = nullptr);

    void updateImage();






private slots:
    void on_pushButtonStart_clicked();

    void finishedLine(QList<Pixel> *);

    void threadFinished();

    void on_pushButtonSaveImg_clicked();

    void on_spinBoxMaxIterations_valueChanged(int arg1);

    void on_comboBox_palette_currentIndexChanged(int index);

    void on_radioButton_normalized_toggled(bool checked);

    void on_radioButton_invert_toggled(bool checked);

    void on_spinBoxW_valueChanged(int arg1);

    void on_spinBoxH_valueChanged(int arg1);

    void on_comboBox_background_color_currentIndexChanged(int index);

    void on_groupBoxMandelFarbe_toggled(bool arg1);

    void on_comboBoxMandelColor_currentIndexChanged(int index);

    void on_comboBox_precession_currentIndexChanged(int index);

    void on_doubleSpinBoxEscapeR_valueChanged(double arg1);

    void on_spinBox_zoom_valueChanged(double arg1);

    void mouse_move_in_img(QPoint pos);

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_rm_history_clicked();

    void on_listWidgetHistory_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_vor_clicked();

    void on_re_valueChanged(double arg1);

    void on_im_valueChanged(double arg1);

    void on_pushButton_copy_clicked();

    void on_pushButton_back_clicked();

    void on_pushButtonHome_clicked();

    void on_comboBox_Fraktal_currentIndexChanged(int index);

    void on_spinBoxFarbWechselIntervall_valueChanged(int arg1);

    void on_spinBoxHSV_satursion_valueChanged(int arg1);

    void on_spinBoxHSV_value_valueChanged(int arg1);

    void on_spinBoxHSV_alpha_valueChanged(int arg1);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_color1_clicked();

    void on_pushButton_color2_clicked();

    void on_pushButton_color3_clicked();

    void on_pushButton_color4_clicked();

    void on_pushButton_color5_clicked();

    void on_radioButtonF1_toggled(bool checked);

    void on_radioButtonF2_toggled(bool checked);

    void on_radioButtonF3_toggled(bool checked);

    void on_radioButtonF4_toggled(bool checked);

    void on_radioButtonF5_toggled(bool checked);

    void on_comboBox_theme_currentIndexChanged(int index);

    void on_pushButtonShowFullScreen_clicked();

    void hideFullScreen();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);


    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);


};








#endif // MAINWINDOW_H