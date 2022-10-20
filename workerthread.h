#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <limits>
#include <complex>
#include <QColor>
#include <QPainter>
#include <QLine>


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
    ImageSetting();
    ImageSetting(double scale, Point midPoint);

    void init(size_t img_w, size_t img_h, size_t maxIterations, bool isMandelbrotSet);
    void cleanUP();
    void setIterationCountAt(ssize_t x, ssize_t y, size_t iterations);
    void setColorSettings(int palette, bool normalized, double logEscape, bool fixedColor, QString fixFraktalColor, bool inverted, double escape_radius);

    size_t getIterationCountAt(QPoint pos);
    size_t getIterationCountAt(ssize_t x, ssize_t y);

    double PixelToImag(QPoint p);
    double PixelToReal(QPoint p);

    QImage * image;
    QPainter * painter;
    Point midPoint;

    double scale,
           logEscape,
            escape_radius;

    ssize_t x_verschiebung;
    ssize_t y_verschiebung;
    ssize_t img_w;
    ssize_t img_h;
    size_t maxIterations;

    QLine xAchse;
    QLine yAchse;

    double juliaStart_img,
           juliaStart_real;

    bool isMandelbrotSet;
    bool isStart = false;

    //Color settings
    bool fixedColor, inverted, normalized, has_hue;
    int palette;
    QString fixFraktalColor;


    double **iterations_normal;
    double **hue;


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

struct Pixel {
    Pixel(ssize_t posX, ssize_t posY, int iterationsTillEscapeR, double z_n_real, double z_n_imag, double normalized_iters, QColor color);
    ssize_t c_x;
    ssize_t c_y;
    size_t iters;
    double normalized_iters;
    double z_n_real,
            z_n_imag;
    QColor color;
};


class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject * parent = nullptr);
    ~WorkerThread();

    ssize_t x_left_corner,
            x_right_corner,
            y_left_corner,
            y_right_corner;

    int calc_mode;
    ImageSetting settings;

    void setRange(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t y_left_corner, ssize_t y_right_corner);

    void startCalc(ImageSetting settings, int calc_mode);

    void run();
    static QColor getPreColor(size_t iters, double normalizedItC, const ImageSetting *imgS);
    static double getNormalizedIterationCount(size_t iters, double z_real, double z_imag, ImageSetting *imS);


signals:
    void finishedLine(QList<Pixel> *p);

};

#endif // WORKERTHREAD_H
