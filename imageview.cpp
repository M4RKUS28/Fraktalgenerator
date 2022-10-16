#include "imageview.h"

#include <QMouseEvent>
#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
{

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

void ImageView::paintEvent(QPaintEvent *)
{
    auto p = QPainter(this);
    // wegen hdpi bildschirmen -> hoch auflösendere Bilder!
        qreal inverseDPR = 1.0 / this->devicePixelRatio();
        p.scale(inverseDPR, inverseDPR);
    // zeichne Bild
    p.drawImage(0,0,img);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event;
}

