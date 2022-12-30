#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <limits>
#include <complex>
#include <QColor>
#include <QPainter>
#include <QLine>

#include "imagesetting.h"


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
    ImageSetting *settings;

    void setRange(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t y_left_corner, ssize_t y_right_corner);

    void startCalc(ImageSetting *settings, int calc_mode);

    void run();
    static QColor getPreColor(size_t iters, double normalizedItC, const ImageSetting *imgS);
    static double getNormalizedIterationCount(size_t iters, double z_real, double z_imag, ImageSetting *imS);
    static QColor getInverse(const QColor &c);


signals:
    void finishedLine(QList<Pixel> *p);

};

#endif // WORKERTHREAD_H
