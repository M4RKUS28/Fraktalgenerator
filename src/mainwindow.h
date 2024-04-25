#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyleFactory>

#include <QLinearGradient>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QColorDialog>
#include <QMap>
#include <QImageWriter>
#include <QProcess>
#include <QAbstractButton>
#include <QMessageBox>



//#define Q_OS_WEB // also in updater!




#ifdef Q_OS_WEB
//#define SINGLE_THREADTED_WEB_ASSEMBLY
#include <emscripten.h>
#else
#include <QProcess>
#endif

#include "updater.h"
#include "workerthread.h"
#include "imageview.h"
#include "dialogimageserie.h"
#pragma once


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#define START_SCALE 220
#define START_POS_X -0.75
#define START_POS_Y 0.0

#define START_POS_X_JUL 0.0
#define START_POS_Y_JUL 0.0

#define init_width_sidebar 351

#define P_VERSION "3.9.8"



class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct ZoomRect{
        ZoomRect();
        void show();
        void hide();
        void updateRectSize(size_t winW, size_t winH, double scale);

        void updateRectPos(QPoint pos, QPointF gauss);

        void setMousePressState(bool isPressed);

        QRect getRect();
        QPoint getMousePos();
        QPointF getMausPosInGauss();

        bool isShown();
        bool rightMouseIsPressed();


    private:
        QPoint mousePos;
        QPointF mousePosInGauss;
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

        LOAD_IMG_FROM_FILE
    };

    const QString STYLE_SHEET_COLOR_BUTTON = "border: 1px solid black; border-radius: 3px; background-color: ";


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    Updater * updater;
    QString defaultStyle;

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
    QList<QList<QColor>> default_colors;
    QColor buttonColors[8];
    bool dont_update_save_check_buttons;
    QColor buttonColors_save[8];
    bool isChecked_save[8];

    QColor currentBackgroundColor;
    QColor fraktalColor;

    void updateButtonColors();
    void setupIconsInCombobox(int width);
    void updateIconInOwnColor(int width);

    QMap<int, bool> keyPressed;

    ImageView * fullScreenView;


    //init load in show Event
    bool codeExecuted;
    // Video MODE:
    DialogImageSerie::ImgSerie imgSerie;
    bool useOldPosForImgSerie;

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

    void startImgFolge();

    void createVideo();

    void loadImageFromSettings(QString path, bool isReadOnly = false);


    void setFullScreenWindowVisible(bool state);

    QColor getFraktalColor();
    void setFratalColor(int index, QColor eigen);

    //color rg folge buttons:
    void radioColorButtonToggeled(int button_num_array, bool status);
    void pushColorButtonClicked(int button_num_array);


private slots:
    void on_pushButtonStart_clicked();

    void finishedLine(QList<Pixel> *);

    void threadFinished();

    void pushButtonSaveImg_clicked();

    void on_spinBoxMaxIterations_valueChanged(int arg1);

    void on_comboBox_palette_currentIndexChanged(int index);

    void on_radioButton_normalized_toggled(bool checked);

    void on_radioButton_invert_toggled(bool checked);

    void on_spinBoxW_valueChanged(int arg1);

    void on_spinBoxH_valueChanged(int arg1);

    void on_groupBoxMandelFarbe_toggled(bool arg1);

    void on_comboBoxMandelColor_currentIndexChanged(int index);

    void on_comboBox_precession_currentIndexChanged(int index);

    void on_doubleSpinBoxEscapeR_valueChanged(double value);

    void on_spinBox_zoom_valueChanged(double arg1);

    void mouse_move_in_img(QPoint pos);

    void mouse_press_in_img(QMouseEvent * ev, QPoint pos);
    void mouse_release_in_img(QMouseEvent * ev, QPoint pos);
    void mouse_double_click_in_img(QMouseEvent * ev, QPoint pos);
    void key_press_in_img(QKeyEvent *);

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_rm_history_clicked();

    void on_listWidgetHistory_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_vor_clicked();

    void on_re_valueChanged(double arg1);

    void on_im_valueChanged(double arg1);

    void pushButton_copy_clicked();

    void on_pushButton_back_clicked();

    void on_pushButtonHome_clicked();

    void on_comboBox_Fraktal_currentIndexChanged(int index);

    void on_spinBoxFarbWechselIntervalln_valueChanged(int arg1);

    void on_spinBoxHSV_satursion_valueChanged(int arg1);

    void on_spinBoxHSV_value_valueChanged(int arg1);

    void on_spinBoxHSV_alpha_valueChanged(int arg1);
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

    void pushButtonShowFullScreen_clicked();

    void on_spinBoxHauptScreen_valueChanged(int arg1);

    void pushButtonOpenImgSerieDialog_clicked();

    void pushButtonOpenVideoDialog_clicked();

    void on_actionBild_speichern_unter_triggered();

    void on_actionBild_in_Zwischenablage_kopieren_triggered();

    void on_actionSeitenmen_anzeigen_toggled(bool arg1);

    void on_actionBilderfolge_erstellen_triggered();

    void on_actionVideo_aus_Bilderfolge_generieren_triggered();

    void on_action_ber_triggered();

    void on_actionVollbildfenster_anzeigen_triggered(bool checked);


    void on_radioButtonKoords_2_clicked(bool checked);

    void on_actionEinstellungen_exportieren_triggered();

    void on_actionEinstellungen_importien_triggered();

    void on_actionBeenden_triggered();

    void on_pushButton_color6_clicked();

    void on_pushButton_color7_clicked();

    void on_pushButton_color8_clicked();

    void on_radioButtonF6_toggled(bool checked);

    void on_radioButtonF7_toggled(bool checked);

    void on_radioButtonF8_toggled(bool checked);

    void on_pushButtonFraktalColor_clicked();

    void on_comboBox_intervall_absolut_currentIndexChanged(int index);

    void on_spinBox_threads_valueChanged(int arg1);

    void on_spinBoxStartVerzoegerung_valueChanged(int arg1);

    void on_actionStandart_Einstellungen_wiederherstellen_triggered();

    void on_spinBoxZahlenfolgeLinienbreite_valueChanged(int arg1);

    void on_spinBoxKoordPenThickness_valueChanged(int arg1);

    void on_comboBoxColorKoordSystem_currentIndexChanged(int index);

    void on_spinBoxBeschriftungen_valueChanged(double arg1);

    void on_doubleSpinBox_Koord_x_start_valueChanged(double arg1);

    void on_doubleSpinBox_Koord_x_end_valueChanged(double arg1);

    void on_spinBoxBeschriftungenY_valueChanged(double arg1);

    void on_doubleSpinBox_Koord_y_start_valueChanged(double arg1);

    void on_doubleSpinBox_Koord_y_end_valueChanged(double arg1);

    void on_comboBox_drawStyle_zahlenfolge_currentIndexChanged(int index);

    void on_comboBox_color_zahlenfolge_currentIndexChanged(int index);

    void on_actionTastenk_rzel_triggered();

    void on_actionBeispiel_1_triggered();

    void on_actionBespiel_2_triggered();

    void on_actionBeispiel_3_triggered();

    void on_UseLogButton_toggled(bool checked);

    void on_logFak_valueChanged(double arg1);

    void on_pushButtonResetUnappliedsettings_clicked();

    void on_actionUnangewandte_Bildeinstellungen_zur_cksetzten_triggered();

    void on_actionBeispiel_4_triggered();

    void on_listWidgetHistory_customContextMenuRequested(const QPoint &pos);

    void context_menue_history_load_clicked();
    void context_menue_history_edit_clicked();
    void context_menue_history_remove_clicked();


    void on_groupBox_2_toggled(bool arg1);

    void on_listWidgetHistory_itemClicked(QListWidgetItem *item);

    void on_actionNach_Updates_suchen_triggered(bool checked);

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


    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);

    // QWidget interface
//protected:
    virtual void showEvent(QShowEvent *event);
};














#endif // MAINWINDOW_H
