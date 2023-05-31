#include "imagesetting.h"
#include "qdebug.h"

#include <QDataStream>
#include <QFile>


ImageSetting::ImageSetting(int id, double scale, double re, double im)
    : gaus_mid_im(im), gaus_mid_re(re), scaleValue(scale), has_hue(false), hue(nullptr), cleandUp(true)
{
    this->id = id;
}

ImageSetting::~ImageSetting()
{
    qDebug() << "~ImageSetting( CLEANED =" << cleandUp << ")";
}

void ImageSetting::cleanUP()
{
//    qDebug() << "~ImageSetting::cleanUP()";
    if(painter) {
        delete painter;
        painter = nullptr;
    }
    if(image) {
        delete image;
        image = nullptr;
    }

    for(int i = 0; i < img_w; i++ )
        delete[] iterations[i];
    delete [] iterations;
    iterations = nullptr;

    for(int i = 0; i < img_w; i++ )
        delete[] iterations_normal[i];
    delete [] iterations_normal;
    iterations_normal = nullptr;

    delete[] NumIterationsPerPixel;
    NumIterationsPerPixel = nullptr;

    if(hueIsSetted()) {
        for(int i = 0; i < img_w; i++ )
            delete[] hue[i];
        delete [] hue;
        qDebug() << "cleanUP hue";
        hue = nullptr;
        this->has_hue = false;
    }

    cleandUp = true;
}


ImageSetting::ImageSetting(int id, QString file_path)
{
    this->id = id;

    this->maxIterations = 0;
    loadFromFileWasOK = true;
    /* open file */
    QFile myFile(file_path);
    qDebug() << "myFile.size()" << myFile.size();


    /* load */
    if(myFile.open(QIODevice::ReadWrite)) {

        QDataStream load(&myFile);
        load.setVersion(QT_DS_VERSION);
        load.setFloatingPointPrecision(QDataStream::DoublePrecision);

        quint32 magic;
        load >> magic;

        if(magic != MAGIC_NUMBER_FILE_VERSION){
                qDebug() << "LOAD DATA FAILED: wrong magic number -> version! " << magic << " != " << MAGIC_NUMBER_FILE_VERSION ;
                loadFromFileWasOK = false;
                return;
        } else {
            qDebug() << "VERSION IS OK: " << MAGIC_NUMBER_FILE_VERSION;
        }

        load >> *this;
        myFile.close();

    } else {
        qDebug() << "OPEN FILE FAILED";
        loadFromFileWasOK = false;
        return;
    }

    qDebug() << scale();

    if(this->maxIterations == 0 || 1.0 / scale() > pow(10.0, 20.0) || this->farbwechselIntervall > 9999999 || this->farbwechselIntervall <= 0) {
        qDebug() << "Got invaild data: " <<  this->maxIterations <<"==" <<0<< "||" <<1.0 / scale()<< ">"<< pow(10.0, 20.0)<< "||"<< this->farbwechselIntervall<< ">"<< 9999999<< "||" <<this->farbwechselIntervall<< "<="<< 0;
        loadFromFileWasOK = false;
        return;
    }

    /// init...
    //: gaus_mid_re(re), gaus_mid_im(im), scaleValue(scale), has_hue(false), hue(nullptr)
    ///init
    // img_w, img_h, this->maxIterations, this->isMandelbrotSet

    // no mem saved
    has_hue = false;


    // FIX ERROR: EXAMPLES have inhalid history: load id from old image -> make it backwardcompatible by overwriting, but removing would be also ok
    this->id = id;


    ///create dynmaic mem:
    this->init(img_w, img_h, this->maxIterations, this->isMandelbrotSet);
}

void ImageSetting::init(size_t img_w, size_t img_h, size_t maxIterations, bool isMandelbrotSet)
{
    this->img_w = img_w;
    this->img_h = img_h;
    this->maxIterations = maxIterations;
    this->isMandelbrotSet = isMandelbrotSet;
    //    isStart = false;

    this->image = new QImage(img_w, img_h, QImage::Format_ARGB32_Premultiplied);
    this->painter = new QPainter(this->image);

    this->re_verscheibung = img_w / 2;
    this->im_verscheibung = img_h / 2;

    this->iterations = new size_t*[img_w];
    for( size_t i = 0; i < img_w; i++) {
        this->iterations[i] = new size_t[img_h];
        memset(this->iterations[i], 0, sizeof(size_t) * img_h );
    }

    this->iterations_normal = new double*[img_w];
    for( size_t i = 0; i < img_w; i++) {
        this->iterations_normal[i] = new double[img_h];
        memset(this->iterations_normal[i], 0, sizeof(double) * img_h );
    }

    // For Histogram
    NumIterationsPerPixel = new size_t[maxIterations + 2];
    memset(NumIterationsPerPixel, 0, sizeof(size_t) * (maxIterations + 2) );

    cleandUp = false;
}

int ImageSetting::store_setting(QString file_path)
{
    /* open file */
    QFile myFile(file_path);

    /* save */
    if(myFile.open(QIODevice::WriteOnly))
    {
        QDataStream save(&myFile);
        save.setVersion(QT_DS_VERSION);
        save.setFloatingPointPrecision(QDataStream::DoublePrecision);
        save << *this;

        if(save.status() != QDataStream::Ok) {
            qDebug() << "Write failed!";
            myFile.close();

            return 2;
        }  else {
            myFile.close();

            qDebug() << "save [debug]: farbwechselIntervall: " << this->farbwechselIntervall;
            return 0;
        }

        myFile.close();
        return 0;
    } else {
        qDebug() << "OPEN FAILED!";
        return 1;
    }
}

void ::ImageSetting::setIterationCountAt(ssize_t x, ssize_t y, size_t iterations)
{
    this->iterations[x][y] = iterations;
}


void ImageSetting::setColorSettings(int farbalgorithmus, bool normalized, double logEscape, int fractalColorIndex, QColor fixFraktalColor, bool inverted, double escape_radius, bool isIntervallVerlauf, bool useLog, int log_fak)
{
    this->normalized = normalized;
    this->logEscape = logEscape;
    this->fraktalColor_index = fractalColorIndex;
    this->fixFraktalColor = fixFraktalColor;
    this->inverted = inverted;
    this->escape_radius = escape_radius;
    this->farbalgorithmus = farbalgorithmus;
    this->isIntervallVerlauf = isIntervallVerlauf;
    this->useLog = useLog;
    this->log_multiplier = log_fak;

}

size_t ImageSetting::getIterationCountAt(QPoint pos)
{
    return this->getIterationCountAt(pos.x(), pos.y());
}

size_t ImageSetting::getIterationCountAt(ssize_t x, ssize_t y)
{
    if(x >= 0 && x < this->img_w && y >= 0 && y < this->img_h)
        return this->iterations[x][y];
    else
        return -1;
}

double ImageSetting::mapImgPosReToGaus(ssize_t img_pos_re)
{
    // mid punkt img == mid punkt gaus -->
    // x verscheibung == mid_punkt_img
    return (((double)img_pos_re - re_verscheibung ) / scale()) + gaus_mid_re;
}

ssize_t ImageSetting::mapGausReToImgPos(double gaus_re)
{
    return (gaus_re - gaus_mid_re) * scale() + re_verscheibung;
}

double ImageSetting::mapImgPosImToGaus(ssize_t img_pos_im)
{
    return (((double)img_pos_im - im_verscheibung ) / scale()) + gaus_mid_im;
}

ssize_t ImageSetting::mapGausImToImgPos(double gaus_im)
{
    return (gaus_im - gaus_mid_im) * scale() + im_verscheibung;
}

double ImageSetting::scale()
{
    return (double)scaleValue;
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
    this->has_hue = true;
}

bool ImageSetting::hueIsSetted()
{
    return has_hue;
}
