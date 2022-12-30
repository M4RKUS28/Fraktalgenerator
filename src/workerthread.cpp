#include "workerthread.h"
#include "qdebug.h"
#include <iostream>


WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
{

}

WorkerThread::~WorkerThread()
{
    if(this->isRunning()) {
        this->requestInterruption();
        this->quit();
        if(this->wait(500) == false) {
            this->terminate();
            if(this->wait(500) == false) {
                qDebug() << "FAILED TO STOP WORKER THREAD!";
            }
        }
    }
}

void WorkerThread::setRange(ssize_t x_left_corner, ssize_t x_right_corner, ssize_t y_left_corner, ssize_t y_right_corner)
{
    this->x_left_corner = x_left_corner;
    this->x_right_corner = x_right_corner;
    this->y_left_corner = y_left_corner;
    this->y_right_corner = y_right_corner;
}

void WorkerThread::startCalc(ImageSetting *settings, int calc_mode)
{
    if (this->isRunning())
        return;

    this->settings = settings;
    this->calc_mode = calc_mode;

    this->start();
}

void WorkerThread::run()
{
    double escapeQuadrat = this->settings->escape_radius * this->settings->escape_radius;
    qDebug() << "  x -> [" << x_left_corner << "\t; " << x_right_corner << "\t] y: [" << y_left_corner << "; " << y_right_corner << "] >> " << escapeQuadrat;

    size_t maxIt = this->settings->maxIterations;

    for(ssize_t x = x_left_corner; x < x_right_corner; x++) {
        QList<Pixel> *line = new QList<Pixel>;
        double z_real, z_imag, c_real, c_imag;

        for(ssize_t y = y_left_corner; y < y_right_corner; y++) {

            if(this->settings->isMandelbrotSet) {
                z_real = 0.0;
                z_imag = 0.0;

                c_real =    settings->mapImgPosReToGaus(x);
                c_imag =  - settings->mapImgPosImToGaus(y);

            } else { // julia Menge
                z_real =   settings->mapImgPosReToGaus(x);
                z_imag = - settings->mapImgPosImToGaus(y);

                c_real =   this->settings->juliaStart_real;
                c_imag =  - this->settings->juliaStart_img;
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

            const double normItCount = getNormalizedIterationCount(i, z_real, z_imag, this->settings);
            line->append(Pixel(x, y, i, z_real, z_imag, normItCount, getPreColor(i, normItCount, this->settings)));
        }
        emit finishedLine(line);
        if(isInterruptionRequested())
            break;
    }

}

QColor WorkerThread::getPreColor(size_t iters, double normalizedItC, const ImageSetting * imgS)
{
    size_t maxIt = imgS->maxIterations;
    size_t farbStufenIntervall = imgS->farbwechselIntervall;
    QColor returnColor = Qt::black;
    unsigned farbstufe = 0, farbStufenAnzahl = imgS->colors.count();
    double anteil = 0.0;

    if(iters > maxIt || maxIt == 0) {
        return Qt::black;
    } if(iters == maxIt) {
        return imgS->inverted ? getInverse( imgS->fixFraktalColor) : imgS->fixFraktalColor;
    }

    //coloring
    switch ( imgS->farbalgorithmus ) {

    case 0 ... 9:
        if(imgS->isIntervallVerlauf) {
            //Intervallverlauf

            if(iters == maxIt || maxIt == 0  || farbStufenAnzahl == 0)
                return Qt::black;

            double iters = normalizedItC;
            // Hier iters % farbStufenIntervall * farbStufenAnzahl für double, um nachdem alle farben dran kamen wieder bei vorne anzufangen
            while( iters > 0 && iters >= farbStufenIntervall * (farbStufenAnzahl))
                iters -= (farbStufenIntervall * (farbStufenAnzahl));

            farbstufe = iters / (farbStufenIntervall);
            //if(iters == )

            iters -= farbStufenIntervall * farbstufe;
            anteil = ((double)iters / (double)farbStufenIntervall);
                                                                                                                // Bei Verlauf zu letzter Farbe farbe  --> starte wieder bei 0
            returnColor = QColor::fromRgb(imgS->colors[farbstufe].second.red() + (double)(imgS->colors[ farbstufe == (farbStufenAnzahl-1) ? 0 : farbstufe + 1].second.red() - imgS->colors[farbstufe].second.red()) * anteil,
                                                       imgS->colors[farbstufe].second.green()  + (double)(imgS->colors[farbstufe == (farbStufenAnzahl-1) ? 0 : farbstufe + 1].second.green() - imgS->colors[farbstufe].second.green()) * anteil,
                                                       imgS->colors[farbstufe].second.blue()  + (double)(imgS->colors[farbstufe == (farbStufenAnzahl-1) ? 0 : farbstufe + 1].second.blue() - imgS->colors[farbstufe].second.blue()) * anteil);
        } else {
            //Absoluter Verlauf
            if(iters == maxIt || maxIt == 0 || farbStufenAnzahl == 0)
                return Qt::black;
            else if(farbStufenAnzahl == 1)
                return imgS->colors[0].second;


            // maximale Iterationszahl aufteilen in Stücke, ein Stück = Farbübergang zu der nächsten Farbe
            // Hier bei absoluten Verlauf: Berechnung der relativen Iterationszahl -> normalizedItC / maxIt.
            //                              Diese zahl mit Gesammtintervallgröße multiplizieren. Da RGB 255 Farben unterstützt, hier 255 gewählt und Gesammtintervall ergibt sich aus Intervallgröße und Anzahl

            // Da es Übergänge sind, gibt es 1 Weniger Farbe --> Wenn z.B. rot und gelb, dann hat man 1 Übergang, von Rot zu Gelb
            double iters = (normalizedItC * 255.0 * (double)( farbStufenAnzahl - 1)) / (double) maxIt ;

            //Nun muss die richtige Farbstufe herausgefunden werden --> durch Teilen der Intervallgröße findet man heraus in welcher Stufe man ist und durch abzug von Stufe*Größe erhält man wieweit iters in dem derzeitigen Intervall sit
            farbstufe = iters / 255; // iters < farbSchritt --> 0, iters < 2*farbSchritt --> 1, ..., wenn farbstufe == farbschrittCount --> iters == max --> black
            iters -= 255 * farbstufe;

            if(farbstufe + 1 >= farbStufenAnzahl)
                return Qt::black;

            // um einen Verlauf zu erzeugen muss gleichermaßen eine farbe zunehmen und eine abnehmen. durch den Anteil der Iteration am Intervallmaximum wird dies erreicht:
            anteil = /*std::min(*/(double)iters / (double)255/*, 1.0)*/;
            // FARBE = START + ( ZIEL - START ) * STEP
            returnColor = QColor::fromRgb(imgS->colors[farbstufe].second.red() + (double)(imgS->colors[farbstufe + 1].second.red() - imgS->colors[farbstufe].second.red()) * anteil,
                                                       imgS->colors[farbstufe].second.green()  + (double)(imgS->colors[farbstufe + 1].second.green() - imgS->colors[farbstufe].second.green()) * anteil,
                                                       imgS->colors[farbstufe].second.blue()  + (double)(imgS->colors[farbstufe + 1].second.blue() - imgS->colors[farbstufe].second.blue()) * anteil);
        }

        break;
    case 10 ... 11:
        returnColor = QColor::fromHsv(int( (358.0 * normalizedItC) / maxIt ), imgS->hsv_saturation,  imgS->spinBoxHSV_value, imgS->spinBoxHSV_alpha);
        break;

    case 12:
        returnColor =  QColor::fromHsv( size_t(pow( (normalizedItC / maxIt) * 359 , 1.5)) % 359, imgS->hsv_saturation,  imgS->spinBoxHSV_value, imgS->spinBoxHSV_alpha);
        break;

    case 13 ... 14:
        returnColor = Qt::white;
        break;

    }

    if(!returnColor.isValid()) {
        return Qt::black;
    } else
        return imgS->inverted ? getInverse( returnColor) : returnColor;
}

double WorkerThread::getNormalizedIterationCount(size_t iters, double z_real, double z_imag, ImageSetting * imS)
{
    const double log2 = 0.69314718055994530941723212145818;//0.30102999566398119521373889472449;
    if(imS->normalized && iters < imS->maxIterations && iters > 1)
        return (double)iters + ((imS->logEscape - log(log(sqrt( (z_real * z_real) + (z_imag * z_imag) ))) ) / log2);
    else
        return iters;
}

QColor WorkerThread::getInverse(const QColor &c)
{
    return QColor::fromRgb( 255 - c.red(), 255 - c.green(), 255 - c.blue() );
}


Pixel::Pixel(ssize_t posX, ssize_t posY, int iterationsTillEscapeR, double z_n_real, double z_n_imag, double normalized_iters, QColor color)
    : c_x(posX), c_y(posY), iters(iterationsTillEscapeR), normalized_iters(normalized_iters), z_n_real(z_n_real), z_n_imag(z_n_imag), color(color)
{

}


Point::Point()
{
    X = 0;
    Y = 0;
}

Point::Point(ssize_t X, ssize_t Y)
    : X(X), Y(Y)
{

}

QPoint Point::rountToQPoint()
{
    return QPoint(x(), y());
}

void Point::setX(ssize_t newX)
{
    X = newX;
}

ssize_t Point::y() const
{
    return Y;
}

void Point::setY(ssize_t newY)
{
    Y = newY;
}

ssize_t Point::x() const
{
    return X;
}


