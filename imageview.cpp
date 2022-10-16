#include "imageview.h"

#include <QMouseEvent>
#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
{
    hdpi_scale = 1.0 / (double)this->devicePixelRatio();
    use_hdpi = true;
}

ImageView::~ImageView()
{

}

void ImageView::setImage(QImage img)
{
    this->img = img;
    this->setFixedSize(img.size());
    this->update();
}

void ImageView::setHdpiScaleEnabled(bool status)
{
    use_hdpi = status;
}

void ImageView::paintEvent(QPaintEvent *)
{
    auto p = QPainter(this);
    // wegen hdpi bildschirmen -> hoch auflösendere Bilder!
    if(use_hdpi)
        p.scale(hdpi_scale, hdpi_scale);
    // zeichne Bild
    p.drawImage(0,0,img);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(event->pos());
}

