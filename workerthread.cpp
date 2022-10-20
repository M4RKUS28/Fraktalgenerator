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

void WorkerThread::startCalc(ImageSetting settings, int calc_mode)
{
    if (this->isRunning())
        return;

    this->settings = settings;
    this->calc_mode = calc_mode;

    this->start();
}

void WorkerThread::run()
{
    double escapeQuadrat = this->settings.escape_radius * this->settings.escape_radius;
    qDebug() << "  x -> [" << x_left_corner << "\t; " << x_right_corner << "\t] y: [" << y_left_corner << "; " << y_right_corner << "] >> " << escapeQuadrat;

    size_t maxIt = this->settings.maxIterations;

    switch ( calc_mode ) {
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
                const double normItCount = getNormalizedIterationCount(i, z.real(), z.imag(), &this->settings);
                line->append(Pixel(x, y, i, z.real(), z.imag(), normItCount, this->getPreColor(i, normItCount, &this->settings)));

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

                size_t i = 0;
                if( (z_real*z_real) + (z_imag*z_imag) <= escapeQuadrat) {
                    for(i = 0; i < maxIt &&  (z_real*z_real) + (z_imag*z_imag) < escapeQuadrat; i++) {
                        const double z_real2 = (z_real * z_real) - (z_imag * z_imag) + c_real;
                        const double z_imag2 = (2.0 * z_real * z_imag) + c_imag;
                        i++;
                        if( (z_real2*z_real2) + (z_imag2*z_imag2) > escapeQuadrat || i > maxIt-1 ) {
                            z_real = z_real2;
                            z_imag = z_imag2;
                            break;
                        }
                        z_real = (z_real2 * z_real2) - (z_imag2 * z_imag2) + c_real;
                        z_imag = (2.0 * z_real2 * z_imag2) + c_imag;
                    }
                }

                const double normItCount = getNormalizedIterationCount(i, z_real, z_imag, &this->settings);
                line->append(Pixel(x, y, i, z_real, z_imag, normItCount, getPreColor(i, normItCount, &this->settings)));
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

                const double normItCount = getNormalizedIterationCount(i, z_real, z_imag, &this->settings);
                line->append(Pixel(x, y, i, z_real, z_imag, normItCount, getPreColor(i, normItCount, &this->settings)));
            }
            emit finishedLine(line);
        }
        break;
    }


}

QColor WorkerThread::getPreColor(size_t iters, double normalizedItC, const ImageSetting * imgS)
{
    double n = normalizedItC;
    double alpha;
    double v = 0;
    int val = 0;
    size_t maxIt = imgS->maxIterations;

    QColor test;

    if(iters == maxIt && imgS->fixedColor) {
        return QColor(imgS->fixFraktalColor);

    } else {
        //coloring
        switch ( imgS->palette ) {
        case 0:
            val = (iters >= maxIt) ? 255 : (int)(n*10.0) % 255;
            test = QColor::fromRgb(0, 0, 0, imgS->inverted ? 255 - val : val );
            if(!test.isValid())
                qDebug() << "WEIL: n=" << n << " und color: " << (imgS->inverted ? 255 - val : val);
            return test;
            break;
        case 1:
            v = (iters >= maxIt) ? 255.0 /*Mandelbrot-Set-Alpha*/ : ((double)((int)(n*3.0) % 255) / 255.0);
            v = imgS->inverted ? 255.0 - v : v;
            return QColor::fromRgbF(1, v, v, 1 );
            break;
        case 3:
//            alpha = (iters >= maxIt) ? 1.0 /*Mandelbrot-Set-Alpha*/ : (double)(std::min( n + ((n*n)/(maxIt/40)), (double)maxIt) / (double)maxIt  ); /*Outside*/
            alpha = (iters >= maxIt) ? 1.0 /*Mandelbrot-Set-Alpha*/ : (double)(std::min( n * ( (1000000/maxIt) / (n + 50)  ), (double)maxIt) / (double)maxIt  ); /*Outside*/
            return QColor::fromRgbF(0, 0, 0, imgS->inverted ? 1.0 - alpha : alpha );
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
        case 6:
            if(iters >= maxIt) {
                return QColor(Qt::black);
            } else {
                return QColor::fromHsl( ((int)std::pow(n / double(maxIt+1) * 360.0, 1.5) % 360), 50, (n / double(maxIt+1)) * 100 );
            }
            break;
        default:
            alpha = (iters >= maxIt) ? 1.0 /*Mandelbrot-Set-Alpha*/ : 0.0; /*Outside*/
            return QColor::fromRgbF(0, 0, 0, imgS->inverted ? 1.0 - alpha : alpha );
            break;
        }
    }

}

double WorkerThread::getNormalizedIterationCount(size_t iters, double z_real, double z_imag, ImageSetting * imS)
{
    const double log2 = 0.69314718055994530941723212145818;//0.30102999566398119521373889472449;
    if(imS->normalized && iters < imS->maxIterations && iters > 1)
        return (double)iters + ((imS->logEscape - log(log(sqrt( (z_real * z_real) + (z_imag * z_imag) ))) ) / log2);
    else
        return iters;
}


Pixel::Pixel(ssize_t posX, ssize_t posY, int iterationsTillEscapeR, double z_n_real, double z_n_imag, double normalized_iters, QColor color)
    : c_x(posX), c_y(posY), iters(iterationsTillEscapeR), normalized_iters(normalized_iters), z_n_real(z_n_real), z_n_imag(z_n_imag), color(color)
{

}


size_t ImageSetting::getNumIterationsPerPixelAt(size_t i) const
{
    return NumIterationsPerPixel[i];
}

void ImageSetting::addNumIterationsPerPixel(size_t iteration)
{
    if(iteration <= maxIterations)
        NumIterationsPerPixel[iteration]++;
}

void ImageSetting::setHue(double **hue, size_t totalIters)
{
    this->hue = hue;
    this->totalIters = totalIters;
}

bool ImageSetting::hueIsSetted()
{
    return has_hue;
}
