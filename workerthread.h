#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <complex>

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject * parent = nullptr);
    ~WorkerThread();

    struct Pixel {
        Pixel(size_t posX, size_t posY, int iterationsTillEscapeR, std::complex<long double> z_n)
            : c_x(posX), c_y(posY), i(iterationsTillEscapeR), z_n(z_n) {}
        size_t c_x;
        size_t c_y;
        size_t i;
        std::complex<long double> z_n;
    };


    ssize_t x_left_corner,
        x_right_corner,
        y_left_corner,
        y_right_corner,
        iters;

    enum PRECESSION {
        DOUBLE,
        LONG_DOUBLE
    }  precession;

    long double zoom,
        escape_radius;


    void startCalc(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t y_left_corner, ssize_t y_right_corner, ssize_t iters, long double escape_radius, long double zoom, PRECESSION p);
    void run();


signals:
    void finishedLine(QList<WorkerThread::Pixel> *p);

};

#endif // WORKERTHREAD_H
