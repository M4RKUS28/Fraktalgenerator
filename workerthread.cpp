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

void WorkerThread::setRange(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t y_left_corner, ssize_t y_right_corner)
{
    this->x_left_corner = x_left_corner;
    this->x_right_corner = x_right_corner;
    this->y_left_corner = y_left_corner;
    this->y_right_corner = y_right_corner;
}

void WorkerThread::startCalc(ImageSetting settings, int calc_mode, int preColorMode)
{
    if (this->isRunning())
        return;

    this->settings = settings;
    this->calc_mode = calc_mode;
    this->preColorMode = preColorMode;
    this->start();
}

void WorkerThread::run()
{
    double escapeQuadrat = this->settings.escape_radius * this->settings.escape_radius;
    qDebug() << "  x -> [" << x_left_corner << "\t; " << x_right_corner << "\t] y: [" << y_left_corner << "; " << y_right_corner << "] >> " << escapeQuadrat;

    size_t maxIt = this->settings.maxIterations;

    switch ( this->calc_mode ) {
    case 1:
        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
            QList<Pixel> *line = new QList<Pixel>;
            std::complex<double> z, c;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {

                if(this->settings.isMandelbrotSet) {
                    z = 0;
                    c = std::complex<double>((double)x / this->settings.scale, - (double)y / this->settings.scale);

                } else { // julia Menge
                    z = std::complex<double>((double)x / this->settings.scale, - (double)y / this->settings.scale);
                    c =  std::complex<double>(this->settings.juliaStart_real, - this->settings.juliaStart_img);
                }

                size_t i = 1;
                for(; i < maxIt && std::abs( (z = z*z + c) ) < this->settings.escape_radius; i++) {}
                const double normItCount = getNormalizedIterationCount(i, z.real(), z.imag());
                line->append(Pixel(x, y, i, z.real(), z.imag(), normItCount, this->getPreColor(i, normItCount)));

            }
            emit finishedLine(line);
        }
        break;

    case 0:
        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
            QList<Pixel> *line = new QList<Pixel>;
            double z_real, z_imag, c_real, c_imag;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {

                if(this->settings.isMandelbrotSet) {
                    z_real = 0.0;
                    z_imag = 0.0;

                    c_real =    (double)x / this->settings.scale;
                    c_imag =  - (double)y / this->settings.scale;

                } else { // julia Menge
                    z_real =   (double)x / this->settings.scale;
                    z_imag = - (double)y / this->settings.scale;

                    c_real =   this->settings.juliaStart_real;
                    c_imag =  - this->settings.juliaStart_img;
                }

                size_t i = 1;
                if( (z_real*z_real) + (z_imag*z_imag) <= escapeQuadrat) {
                    for(; i < maxIt; i++) {
                        const double z_real2 = (z_real * z_real) - (z_imag * z_imag) + c_real;
                        const double z_imag2 = (2.0 * z_real * z_imag) + c_imag;
                        if( (z_real2*z_real2) + (z_imag2*z_imag2) > escapeQuadrat || i >= maxIt ) {
                            z_real = z_real2;
                            z_imag = z_imag2;
                            break;
                        } else {
                            i++;
                        }
                        z_real = (z_real2 * z_real2) - (z_imag2 * z_imag2) + c_real;
                        z_imag = (2.0 * z_real2 * z_imag2) + c_imag;
                        if( (z_real*z_real) + (z_imag*z_imag) > escapeQuadrat )
                            break;
                    }
                }

                const double normItCount = getNormalizedIterationCount(i, z_real, z_imag);
                line->append(Pixel(x, y, i, z_real, z_imag, normItCount, getPreColor(i, normItCount)));
            }
            emit finishedLine(line);
        }
        break;
    case 2:
        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
            QList<Pixel> *line = new QList<Pixel>;
            double z_real, z_imag, c_real, c_imag;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {

                if(this->settings.isMandelbrotSet) {
                    z_real = 0.0;
                    z_imag = 0.0;

                    c_real =    (double)x / this->settings.scale;
                    c_imag =  - (double)y / this->settings.scale;

                } else {
                    z_real =   (double)x / this->settings.scale;
                    z_imag = - (double)y / this->settings.scale;

                    c_real =   this->settings.juliaStart_real;
                    c_imag =  - this->settings.juliaStart_img;
                }

                size_t i = 1;
                for(; i < maxIt; i++) {
                    const double z_real2 = (z_real * z_real) - (z_imag * z_imag) + c_real;
                    const double z_imag2 = (2.0 * z_real * z_imag) + c_imag;
                    if(((z_real*z_real) + (c_imag*c_imag)) > escapeQuadrat)
                        break;
                    z_real = (z_real2 * z_real2) - (z_imag2 * z_imag2) + c_real;
                    z_imag = (2.0 * z_real2 * z_imag2) + c_imag;
                    if( ((z_real*z_real) + (c_imag*c_imag)) > escapeQuadrat)
                        break;
                }

                const double normItCount = getNormalizedIterationCount(i, z_real, z_imag);
                line->append(Pixel(x, y, i, z_real, z_imag, normItCount, getPreColor(i, normItCount)));
            }
            emit finishedLine(line);
        }
        break;
    }


}

QColor WorkerThread::getPreColor(size_t iters, double normalizedItC)
{
    double n = normalizedItC;
    double alpha;
    double v = 0;
    int val = 0;
    size_t maxIt = this->settings.maxIterations;

    if(iters == maxIt && this->settings.fixedColor) {
        return QColor(this->settings.fixFraktalColor);

    } else {
        //coloring
        switch ( preColorMode ) {
        case 0:
            val = (iters >= maxIt) ? 255 : (int)(n*10.0) % 255;
            return QColor::fromRgb(0, 0, 0, this->settings.inverted ? 255 - val : val );
            break;
        case 1:
            v = (iters >= maxIt) ? 255.0 /*Mandelbrot-Set-Alpha*/ : ((double)((int)(n*3.0) % 255) / 255.0);
            v = this->settings.inverted ? 255.0 - v : v;
            return QColor::fromRgbF(1, v, v, 1 );
            break;
        case 3:
            alpha = (iters >= maxIt) ? 1.0 /*Mandelbrot-Set-Alpha*/ : (double)((int)(n) % maxIt) / ((double)maxIt); /*Outside*/
            return QColor::fromRgbF(0, 0, 0, this->settings.inverted ? 1.0 - alpha : alpha );
            break;
        case 4:
            if(iters >= maxIt) {
                return QColor(Qt::black);
            } else {
                // hsv = [powf((i / max) * 360, 1.5) % 360, 100, (i / max) * 100]
                QColor c = QColor::fromHsv( size_t(pow( (n / maxIt) * 358 , 1.5)) % 358, 100, (n / maxIt) * 250);
                return c;
            }
            break;
        case 5:
            if(iters >= maxIt) {
                return QColor(Qt::black);
            } else {
                return QColor::fromHsv(int( (358.0 * n) / maxIt ), 255, 255);
            }
            break;
        default:
            alpha = (iters >= maxIt) ? 1.0 /*Mandelbrot-Set-Alpha*/ : 0.0; /*Outside*/
            return QColor::fromRgbF(0, 0, 0, this->settings.inverted ? 1.0 - alpha : alpha );
            break;
        }
    }

}

double WorkerThread::getNormalizedIterationCount(size_t iters, double z_real, double z_imag)
{
    if(this->settings.normalized && iters < this->settings.maxIterations)
        return (double)iters + ((this->settings.logEscape - log(log(sqrt( (z_real * z_real) + (z_imag * z_imag) ))) ) / this->log2);
    else
        return iters;
}


Pixel::Pixel(ssize_t posX, ssize_t posY, int iterationsTillEscapeR, double z_n_real, double z_n_imag, double normalized_iters, QColor color)
    : c_x(posX), c_y(posY), iters(iterationsTillEscapeR), normalized_iters(normalized_iters), z_n_real(z_n_real), z_n_imag(z_n_imag), color(color)
{

}
