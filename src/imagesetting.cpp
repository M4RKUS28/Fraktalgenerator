#include "imagesetting.h"
#include "qdebug.h"

#include <QFile>


ImageSetting::ImageSetting(int id, double scale, double re, double im)
    : gaus_mid_im(im), gaus_mid_re(re), scaleValue(scale), has_hue(false), hue(nullptr)
{
    this->id = id;
}


ImageSetting::ImageSetting(int id, QString file_path)
{
    this->id = id;
    this->maxIterations = 0;
    loadFromFileWasOK = true;
    /* open file */
    QFile myFile(file_path);


    /* load */
    if(myFile.open(QIODevice::ReadWrite)) {
        QDataStream load(&myFile);
        load.setFloatingPointPrecision(QDataStream::DoublePrecision);
        load.setVersion(QDataStream::Qt_5_12);
        load >> *this;

        if(load.status() == QDataStream::Ok)
        {
            // read is ok, check the data you had read


        }
        else
        {
            // wait for more data or show unknown error
            qDebug() << "LOAD DATA FAILED!" << load.status();
            loadFromFileWasOK = false;
            return;

        }

        myFile.close();
    } else {
        qDebug() << "OPEN FILE FAILED";
        loadFromFileWasOK = false;
        return;
    }

    qDebug() << scale();

    if(this->maxIterations == 0) {
        qDebug() << "Got invaild data";
        loadFromFileWasOK = false;
        return;
    }

    /// init...
    //: gaus_mid_re(re), gaus_mid_im(im), scaleValue(scale), has_hue(false), hue(nullptr)
    ///init
    // img_w, img_h, this->maxIterations, this->isMandelbrotSet

    // no mem saved
    has_hue = false;

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

}

int ImageSetting::store_setting(QString file_path)
{
    /* open file */
    QFile myFile(file_path);

    /* save */
    if(myFile.open(QIODevice::WriteOnly))
    {
        QDataStream save(&myFile);
        save.setVersion(QDataStream::Qt_5_12);
        save.setFloatingPointPrecision(QDataStream::DoublePrecision);
        save << *this;

        if(save.status() != QDataStream::Ok) {
            qDebug() << "Write failed!";
            myFile.close();

            return 2;
        }  else {
            myFile.close();
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


void ImageSetting::setColorSettings(int farbalgorithmus, bool normalized, double logEscape, int fractalColorIndex, QColor fixFraktalColor, bool inverted, double escape_radius, bool isIntervallVerlauf)
{
    this->normalized = normalized;
    this->logEscape = logEscape;
    this->fraktalColor_index = fractalColorIndex;
    this->fixFraktalColor = fixFraktalColor;
    this->inverted = inverted;
    this->escape_radius = escape_radius;
    this->farbalgorithmus = farbalgorithmus;
    this->isIntervallVerlauf = isIntervallVerlauf;
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
