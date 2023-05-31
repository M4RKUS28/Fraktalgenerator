#ifndef IMAGESETTING_H
#define IMAGESETTING_H


#include <QThread>
#include <limits>
#include <complex>
#include <QColor>
#include <QPainter>
#include <QLine>
#include <QDataStream>

#define MAGIC_NUMBER_FILE_VERSION 0xAC11C9DE
#define QT_DS_VERSION QDataStream::Qt_5_12


struct ImageSetting {
    ImageSetting(int id, double scale, double re, double im);
    ~ImageSetting();

    bool loadFromFileWasOK = true;
    ImageSetting(int id, QString file_path);


    void init(size_t img_w, size_t img_h, size_t maxIterations, bool isMandelbrotSet);
    int store_setting(QString file_path);

    void cleanUP();
    void setIterationCountAt(ssize_t x, ssize_t y, size_t iterations);


    void setColorSettings(int farbalgorithmus, bool normalized, double logEscape, int fractalColorIndex, QColor fixFraktalColor,
                          bool inverted, double escape_radius, bool isIntervallVerlauf, bool useLog, int log_fak);

    size_t getIterationCountAt(QPoint pos);
    size_t getIterationCountAt(ssize_t x, ssize_t y);

    double mapImgPosReToGaus(ssize_t img_pos_re);
    ssize_t mapGausReToImgPos(double gaus_re);

    double mapImgPosImToGaus(ssize_t img_pos_im);
    ssize_t mapGausImToImgPos(double gaus_im);



    double scale();

    int id;

    double gaus_mid_im;
    double im_verscheibung;

    double gaus_mid_re;
    double re_verscheibung;

    QImage * image;
    QPainter * painter;

    double scaleValue,
           logEscape,
           escape_radius;

    long long img_w;
    long long img_h;
    unsigned long long maxIterations;

    double juliaStart_img,
           juliaStart_real;

    bool isMandelbrotSet,
        useLog;

    int farbalgorithmus,
        log_multiplier;

    //Color settings
    bool inverted, normalized, has_hue, isIntervallVerlauf;
    int fraktalColor_index;
    QColor fixFraktalColor;
    QList<QPair<int, QColor>> colors;
    unsigned hsv_saturation, spinBoxHSV_value, spinBoxHSV_alpha,
                    farbwechselIntervall;

    double **iterations_normal;
    double **hue;


    bool cleandUp;

    /* overload the operators */
    friend QDataStream &operator<< (QDataStream &out, const ImageSetting &rhs)
    {
        out << MAGIC_NUMBER_FILE_VERSION << rhs.id << rhs.scaleValue << rhs.gaus_mid_re << rhs.gaus_mid_im << rhs.img_w << rhs.img_h << rhs.maxIterations << rhs.isMandelbrotSet
            << rhs.logEscape << rhs.escape_radius << rhs.juliaStart_img << rhs.juliaStart_real << rhs.fraktalColor_index << rhs.inverted << rhs.normalized
            << rhs.farbalgorithmus << rhs.fixFraktalColor << rhs.colors << rhs.hsv_saturation << rhs.spinBoxHSV_value << rhs.isIntervallVerlauf
            << rhs.spinBoxHSV_alpha << rhs.farbwechselIntervall << rhs.useLog << rhs.log_multiplier;
        return out;
    }

    friend QDataStream &operator>> (QDataStream &in, ImageSetting &rhs)
    {
        in >> rhs.id >> rhs.scaleValue >> rhs.gaus_mid_re >> rhs.gaus_mid_im >> rhs.img_w >> rhs.img_h >> rhs.maxIterations >> rhs.isMandelbrotSet
            >> rhs.logEscape >> rhs.escape_radius >> rhs.juliaStart_img >> rhs.juliaStart_real >> rhs.fraktalColor_index >> rhs.inverted >> rhs.normalized
            >> rhs.farbalgorithmus >> rhs.fixFraktalColor >>rhs.colors >> rhs.hsv_saturation >> rhs.spinBoxHSV_value << rhs.isIntervallVerlauf
            >> rhs.spinBoxHSV_alpha >> rhs.farbwechselIntervall >> rhs.useLog >> rhs.log_multiplier;
        return in;
    }


public:
    size_t getNumIterationsPerPixelAt(size_t i) const;
    void addNumIterationsPerPixel(size_t iteration);

    void setHue(double ** hue, size_t totalIters );
    bool hueIsSetted();
private:
    size_t **iterations;
    size_t * NumIterationsPerPixel;
    size_t totalIters;

};

#endif // IMAGESETTING_H
