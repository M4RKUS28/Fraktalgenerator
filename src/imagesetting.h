#ifndef IMAGESETTING_H
#define IMAGESETTING_H


#include <QThread>
#include <limits>
#include <complex>
#include <QColor>
#include <QPainter>
#include <QLine>
#include <QDataStream>



struct ImageSetting {
    ImageSetting(int id, double scale, double re, double im);

    bool loadFromFileWasOK = true;
    ImageSetting(int id, QString file_path);


    void init(size_t img_w, size_t img_h, size_t maxIterations, bool isMandelbrotSet);
    int store_setting(QString file_path);

    void cleanUP();
    void setIterationCountAt(ssize_t x, ssize_t y, size_t iterations);
    void setColorSettings(int palette, bool normalized, double logEscape, bool fixedColor, QString fixFraktalColor,
                          bool inverted, double escape_radius, QString backgroundColor);

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

    ssize_t img_w;
    ssize_t img_h;
    size_t maxIterations;

    double juliaStart_img,
           juliaStart_real;

    bool isMandelbrotSet;

    //Color settings
    bool fixedColor, inverted, normalized, has_hue;
    int palette;
    QString fixFraktalColor, backgroundColor;

    QList<std::pair<int,QColor>> colors;
    unsigned hsv_saturation, spinBoxHSV_value, spinBoxHSV_alpha, farbwechselIntervall;

    double **iterations_normal;
    double **hue;


    /* overload the operators */
    friend QDataStream &operator<< (QDataStream &out, const ImageSetting &rhs)
    {
        out << rhs.id << rhs.scaleValue << rhs.gaus_mid_re << rhs.gaus_mid_im << rhs.img_w << rhs.img_h << rhs.maxIterations << rhs.isMandelbrotSet
            << rhs.logEscape << rhs.escape_radius << rhs.juliaStart_img << rhs.juliaStart_real << rhs.fixedColor << rhs.inverted << rhs.normalized
            << rhs.palette << rhs.fixFraktalColor << rhs.backgroundColor << rhs.colors << rhs.hsv_saturation << rhs.spinBoxHSV_value
            << rhs.spinBoxHSV_alpha << rhs.farbwechselIntervall;
        return out;
    }

    friend QDataStream &operator>> (QDataStream &in, ImageSetting &rhs)
    {
        in >> rhs.id >> rhs.scaleValue >> rhs.gaus_mid_re >> rhs.gaus_mid_im >> rhs.img_w >> rhs.img_h >> rhs.maxIterations >> rhs.isMandelbrotSet
            >> rhs.logEscape >> rhs.escape_radius >> rhs.juliaStart_img >> rhs.juliaStart_real >> rhs.fixedColor >> rhs.inverted >> rhs.normalized
            >> rhs.palette >> rhs.fixFraktalColor >> rhs.backgroundColor >> rhs.colors >> rhs.hsv_saturation >> rhs.spinBoxHSV_value
            >> rhs.spinBoxHSV_alpha >> rhs.farbwechselIntervall;
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
