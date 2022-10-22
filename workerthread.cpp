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
            std::complex<long double> z, c;

            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {

                if(this->settings.isMandelbrotSet) {
                    z = 0;
                    c = std::complex<long double>(settings.mapImgPosReToGausLong(x),
                                             - settings.mapImgPosImToGausLong(y));

                } else { // julia Menge
                    z = std::complex<long double>(settings.mapImgPosReToGausLong(x),
                                             - settings.mapImgPosImToGausLong(y));
                    c =  std::complex<long double>(this->settings.juliaStart_real,
                                              - this->settings.juliaStart_img);
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

                    c_real =    settings.mapImgPosReToGaus(x);
                    c_imag =  - settings.mapImgPosImToGaus(y);;

                } else { // julia Menge
                    z_real =   settings.mapImgPosReToGaus(x);
                    z_imag = - settings.mapImgPosImToGaus(y);

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
//    case 2:
//        for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
//            QList<Pixel> *line = new QList<Pixel>;
//            double z_real, z_imag, c_real, c_imag;

//            for(ssize_t y = y_left_corner; y < y_right_corner; y++) {

//                if(this->settings.isMandelbrotSet) {
//                    z_real = 0.0;
//                    z_imag = 0.0;

//                    c_real =    settings.mapImgPosReToGaus(x);
//                    c_imag =  - settings.mapImgPosImToGaus(y);;

//                } else { // julia Menge
//                    z_real =   settings.mapImgPosReToGaus(x);
//                    z_imag = - settings.mapImgPosImToGaus(y);

//                    c_real =   this->settings.juliaStart_real;
//                    c_imag =  - this->settings.juliaStart_img;
//                }

//                size_t i = 1;
//                for(; i < maxIt; i++) {
//                    const double z_real2 = (z_real * z_real) - (z_imag * z_imag) + c_real;
//                    const double z_imag2 = (2.0 * z_real * z_imag) + c_imag;
//                    if(((z_real*z_real) + (c_imag*c_imag)) > escapeQuadrat)
//                        break;
//                    z_real = (z_real2 * z_real2) - (z_imag2 * z_imag2) + c_real;
//                    z_imag = (2.0 * z_real2 * z_imag2) + c_imag;
//                    if( ((z_real*z_real) + (c_imag*c_imag)) > escapeQuadrat)
//                        break;
//                }

//                const double normItCount = getNormalizedIterationCount(i, z_real, z_imag, &this->settings);
//                line->append(Pixel(x, y, i, z_real, z_imag, normItCount, getPreColor(i, normItCount, &this->settings)));
//            }
//            emit finishedLine(line);
//        }
//        break;
    }


}

QColor WorkerThread::getPreColor(size_t iters, double normalizedItC, const ImageSetting * imgS)
{
    double n = normalizedItC;
    double alpha;
    int val = 0;

    size_t maxIt = imgS->maxIterations;

    double anteil;
    size_t farbSchritt = imgS->farbwechselIntervall;
    QColor returnColor = Qt::black;
    unsigned farbstufe = 0,
            farbschrittCount = 3; // später auch erweiterbar mit mehreren farben

    if(iters > maxIt || maxIt == 0) {
        return Qt::black;
    }

    if(iters == maxIt && imgS->fixedColor) {
        return QColor(imgS->fixFraktalColor);
    }

    //coloring
    switch ( imgS->palette ) {
    case 0:
        val = (iters >= maxIt) ? 255 : (int)(n*10.0) % 255;
        returnColor = QColor::fromRgb(0, 0, 0, imgS->inverted ? 255 - val : val );
        break;

    case 6:
    case 3: // farblos
        returnColor = (iters == maxIt) ? (imgS->inverted ? Qt::white : Qt::black) : (imgS->inverted ? Qt::black : Qt::white);
        break;

    case 4:
      returnColor =  (iters == maxIt) ? Qt::black : QColor::fromHsv( size_t(pow( (n / maxIt) * 359 , 1.5)) % 359,
                                                                     imgS->hsv_saturation,  imgS->spinBoxHSV_value, imgS->spinBoxHSV_alpha);
        break;

    case 5:
        returnColor = (iters == maxIt) ? Qt::black : QColor::fromHsv(int( (358.0 * n) / maxIt ), imgS->hsv_saturation,  imgS->spinBoxHSV_value, imgS->spinBoxHSV_alpha);
        break;


    case 2:
        if(iters == maxIt || maxIt == 0)
            return Qt::black;

        iters = normalizedItC;
//        while( iters > 0 && iters > farbSchritt * farbschrittCount)
//            iters -= (farbSchritt * farbschrittCount);
        iters %= farbSchritt * farbschrittCount;

        farbstufe = iters / farbSchritt; // iters < farbSchritt --> 0, iters < 2*farbSchritt --> 1, ..., wenn farbstufe == farbschrittCount --> iters == max --> black
        if(farbstufe >= farbschrittCount)
            returnColor = Qt::black;
        else {
            iters -= farbSchritt * farbstufe;
            anteil = ((double)iters / (double)farbSchritt);
            // FARBE = START + ( ZIEL - START ) * STEP
            returnColor = QColor::fromRgb(imgS->rgb1[farbstufe].red() + (double)(imgS->rgb1[farbstufe == 2 ? 0 : farbstufe + 1].red() - imgS->rgb1[farbstufe].red()) * anteil,
                                                       imgS->rgb1[farbstufe].green()  + (double)(imgS->rgb1[farbstufe == 2 ? 0 : farbstufe + 1].green() - imgS->rgb1[farbstufe].green()) * anteil,
                                                       imgS->rgb1[farbstufe].blue()  + (double)(imgS->rgb1[farbstufe == 2 ? 0 : farbstufe + 1].blue() - imgS->rgb1[farbstufe].blue()) * anteil);
        }
        break;

    case 1:
        if(iters == maxIt || maxIt == 0)
            return Qt::black;

        iters = (iters * farbSchritt * farbschrittCount) / maxIt ;

        farbstufe = iters / farbSchritt; // iters < farbSchritt --> 0, iters < 2*farbSchritt --> 1, ..., wenn farbstufe == farbschrittCount --> iters == max --> black
        if(farbstufe >= farbschrittCount)
            returnColor = Qt::black;
        else {
            iters -= farbSchritt * farbstufe;
            anteil = std::min((double)iters / (double)farbSchritt, 1.0);
            returnColor = QColor::fromRgb(imgS->rgb1[farbstufe].red() + (double)(imgS->rgb1[farbstufe == 2 ? 0 : farbstufe + 1].red() - imgS->rgb1[farbstufe].red()) * anteil,
                                                       imgS->rgb1[farbstufe].green()  + (double)(imgS->rgb1[farbstufe == 2 ? 0 : farbstufe + 1].green() - imgS->rgb1[farbstufe].green()) * anteil,
                                                       imgS->rgb1[farbstufe].blue()  + (double)(imgS->rgb1[farbstufe == 2 ? 0 : farbstufe + 1].blue() - imgS->rgb1[farbstufe].blue()) * anteil);
        }
        break;

//        if( iters < farbSchritt) { // farbstufe 1
//            anteil = ((double)iters / (double)farbSchritt);

//            return QColor::fromRgb(imgS->rgb1[0].red() + (double)(imgS->rgb1[1].red() - imgS->rgb1[0].red()) * anteil,
//                                   imgS->rgb1[0].green()  + (double)(imgS->rgb1[1].green() - imgS->rgb1[0].green()) * anteil,
//                                   imgS->rgb1[0].blue()  + (double)(imgS->rgb1[1].blue() - imgS->rgb1[0].blue()) * anteil);

//        } else if( iters < farbSchritt*2) { // farbstufe 2
//            iters -= farbSchritt;
//            anteil = ((double)iters / (double)farbSchritt);

//            return QColor::fromRgb(imgS->rgb1[1].red()  + (imgS->rgb1[2].red() - imgS->rgb1[1].red()) * anteil,
//                                   imgS->rgb1[1].green()  + (imgS->rgb1[2].green() - imgS->rgb1[1].green()) * anteil,
//                                   imgS->rgb1[1].blue()  + (imgS->rgb1[2].blue() - imgS->rgb1[1].blue()) * anteil);
//        } else if( iters < farbSchritt*3) { //  farbstufe 3
//            iters -= farbSchritt*2;
//            anteil = ((double)iters / (double)farbSchritt);

//            return QColor::fromRgb(imgS->rgb1[2].red()  + (imgS->rgb1[0].red() - imgS->rgb1[2].red()) * anteil,
//                                   imgS->rgb1[2].green()  + (imgS->rgb1[0].green() - imgS->rgb1[2].green()) * anteil,
//                                   imgS->rgb1[2].blue()  + (imgS->rgb1[0].blue() - imgS->rgb1[2].blue()) * anteil);
//        } else return QColor(Qt::black);

    default:
        alpha = (iters >= maxIt) ? 1.0 /*Mandelbrot-Set-Alpha*/ : 0.0; /*Outside*/
        return QColor::fromRgbF(0, 0, 0, imgS->inverted ? 1.0 - alpha : alpha );
    }


//        case 7:
//            iters = (iters * farbSchritt * 3) / maxIt ;
//            iters %= (farbSchritt * 3);

//            if( iters < farbSchritt) { // farbstufe 1
//                anteil = (iters / farbSchritt);
//                // FARBE = START + ( ZIEL - START ) * STEP

//                return QColor::fromRgb(iters , 0, 255 - iters);

//            } else if( iters < farbSchritt*2) { // farbstufe 2
//                iters -= farbSchritt;
//                anteil = (iters / farbSchritt) * 255;

//                return QColor::fromRgb(255 - iters, iters, 0);

//            } else if( iters < farbSchritt*3) { //  farbstufe 3
//                iters -= farbSchritt*2;
//                anteil = (iters / farbSchritt) * 255;

//                return QColor::fromRgb(0, 255 - iters, iters);

//            } else return QColor(Qt::black);
//        case 7:
//            if(iters == maxIt)
//                return Qt::black;

//            iters = (iters * farbSchritt * 3) / maxIt ;
//            iters %= (farbSchritt * 3);

//            if( iters < farbSchritt) { // farbstufe 1
//                anteil = ((double)iters / (double)farbSchritt);
//                // FARBE = START + ( ZIEL - START ) * STEP

//                return QColor::fromRgb((double)imgS->rgb1[0].red() * anteil+ (double)(imgS->rgb1[2].red() - imgS->rgb1[0].red()) * anteil,
//                                       (double)imgS->rgb1[0].green() * anteil + (double)(imgS->rgb1[2].green() - imgS->rgb1[0].green()) * anteil,
//                                       (double)imgS->rgb1[0].blue() * anteil + (double)(imgS->rgb1[2].blue() - imgS->rgb1[0].blue()) * anteil);

//            } else if( iters < farbSchritt*2) { // farbstufe 2
//                iters -= farbSchritt;
//                anteil = ((double)iters / (double)farbSchritt);

//                return QColor::fromRgb((double)imgS->rgb1[1].red() * anteil + (imgS->rgb1[0].red() - imgS->rgb1[1].red()) * anteil,
//                                       (double)imgS->rgb1[1].green() * anteil + (imgS->rgb1[0].green() - imgS->rgb1[1].green()) * anteil,
//                                       (double)imgS->rgb1[1].blue() * anteil + (imgS->rgb1[0].blue() - imgS->rgb1[1].blue()) * anteil);
//            } else if( iters < farbSchritt*3) { //  farbstufe 3
//                iters -= farbSchritt*2;
//                anteil = ((double)iters / (double)farbSchritt);

//                return QColor::fromRgb((double)imgS->rgb1[2].red() * anteil + (imgS->rgb1[1].red() - imgS->rgb1[2].red()) * anteil,
//                                       (double)imgS->rgb1[2].green() * anteil + (imgS->rgb1[1].green() - imgS->rgb1[2].green()) * anteil,
//                                       (double)imgS->rgb1[2].blue() * anteil + (imgS->rgb1[1].blue() - imgS->rgb1[2].blue()) * anteil);
//            } else return QColor(Qt::black);



//        }
//    }

    return returnColor.isValid() ? returnColor : Qt::black;
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
