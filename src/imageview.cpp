#include "imageview.h"

#include <QMouseEvent>
#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
{
    // auto
    hdpi_multiplicator = (double)this->devicePixelRatio();
    hdpi_scale = 1.0 / hdpi_multiplicator;
}


ImageView::~ImageView()
{

}

void ImageView::setImage(QImage img)
{
    this->img = img;
    this->setFixedSize(img.size() * hdpi_scale);
    this->update();
}

void ImageView::setHdpiScale(double multiplikator)
{
    hdpi_multiplicator = multiplikator;
    hdpi_scale = 1.0 / hdpi_multiplicator;
}

double ImageView::getHdpi_scale() const
{
    return hdpi_scale;
}

double ImageView::getHdpi_multiplicator() const
{
    return hdpi_multiplicator;
}

const QImage &ImageView::getImg() const
{
    return img;
}


void ImageView::paintEvent(QPaintEvent *)
{
    auto p = QPainter(this);
    // wegen hdpi bildschirmen -> hoch auflösendere Bilder!
    p.scale(hdpi_scale, hdpi_scale);

    // zeichne Bild
    p.drawImage(0,0,img);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(event->pos() * hdpi_multiplicator);
}


//void ImageView::mousePressEvent(QMouseEvent *event)
//{
//    qDebug() << "press";
//}

//void ImageView::mouseReleaseEvent(QMouseEvent *event)
//{
//    qDebug() << "ress";
//}

void ImageView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape || event->key() == Qt::Key_Q || event->key() == Qt::Key_L || event->key() == Qt::Key_E)
        emit escapePressed();
}
