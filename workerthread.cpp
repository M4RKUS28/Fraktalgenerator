#include "workerthread.h"
#include "qdebug.h"

WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
{

}

WorkerThread::~WorkerThread()
{

}

void WorkerThread::startCalc(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t y_left_corner, ssize_t y_right_corner, ssize_t iters, long double escape_radius, long double zoom, PRECESSION p)
{

    if (this->isRunning())
        return;


    this->x_left_corner = x_left_corner;
    this->x_right_corner = x_right_corner;
    this->y_left_corner = y_left_corner;
    this->y_right_corner = y_right_corner;

    this->zoom = zoom;
    this->iters = iters;
    this->escape_radius = escape_radius;
    this->precession = p;

    this->start();
}

void WorkerThread::run()
{
    // qDebug() << "  x -> [" << x_left_corner << "\t; " << x_right_corner << "\t] y: [" << y_left_corner << "; " << y_right_corner << "]";

    switch ( this->precession ) {
    case WorkerThread::DOUBLE:
        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
            QList<Pixel> *line = new QList<Pixel>;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {
                std::complex<double> c(x * zoom, - y * zoom);
                std::complex<double> z = 0;
                ssize_t i;
                for( i = 0; i < iters && std::abs( (z = z*z + c) ) < escape_radius; i++);
                if( i == iters ) {
                    line->append(Pixel(x, y, 0, z));
                } else {
                    line->append(Pixel(x, y, i, z));
                }
            }
            emit finishedLine(line);
        }
        break;

    case WorkerThread::LONG_DOUBLE:
        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
            QList<Pixel> *line = new QList<Pixel>;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {
                std::complex<long double> c(x * zoom, - y * zoom);
                std::complex<long double> z = 0;
                ssize_t i;
                for( i = 0; i < iters && std::abs( (z = z*z + c) ) < escape_radius; i++);
                if( i == iters ) {
                    line->append(Pixel(x, y, 0, z));
                } else {
                    line->append(Pixel(x, y, i, z));
                }
            }
            emit finishedLine(line);
        }
        break;
    }


}

