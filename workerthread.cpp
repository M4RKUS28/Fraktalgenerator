#include "workerthread.h"
#include "qdebug.h"
#include <iostream>


WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
{

}

WorkerThread::~WorkerThread()
{

}
#include <limits>

void WorkerThread::startCalc(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t test, ssize_t y_left_corner, ssize_t y_right_corner,
                             ssize_t iters, long double escape_radius, long double zoom, PRECESSION p, bool mandelbrot, std::complex<long double> startJulia)
{

    if (this->isRunning())
        return;

//    std::cout << "x test" << test << std::endl;

    this->x_left_corner = x_left_corner;
    this->x_right_corner = x_right_corner;
    this->y_left_corner = y_left_corner;
    this->y_right_corner = y_right_corner;

    this->zoom = zoom;
    this->iters = iters;
    this->escape_radius = escape_radius;
    this->precession = p;
    this->isMandelbrot = mandelbrot;
    this->startJulia = startJulia;

    this->start();
}


void WorkerThread::run()
{
    qDebug() << "  x -> [" << x_left_corner << "\t; " << x_right_corner << "\t] y: [" << y_left_corner << "; " << y_right_corner << "]";

    switch ( this->precession ) {
    case WorkerThread::DOUBLE:
        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
            QList<Pixel> *line = new QList<Pixel>;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {
                std::complex<double> z, c;
                if(isMandelbrot /*mandelbrot-menge*/) {
                    z = 0;
                    c = std::complex<double>((double)x * zoom, - (double)y * zoom);

                } else { // julia Menge
                    z = std::complex<double>((double)x * zoom, - (double)y * zoom);
                    c = startJulia;
                }
                ssize_t i;
                for( i = 1; i < iters && std::abs( (z = z*z + c) ) < escape_radius; i++) {}

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
                std::complex<long double> z, c;
                if(isMandelbrot /*mandelbrot-menge*/) {
                    z = 0;
                    c = std::complex<long double>((long double)x * zoom, - (long double)y * zoom);

                } else { // julia Menge
                    z = std::complex<long double>((long double)x * zoom, - (long double)y * zoom);
                    c = startJulia;
                }

                ssize_t i;
                for( i = 1; i < iters && std::abs( (z = z*z + c) ) < escape_radius; i++) {

                }
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

