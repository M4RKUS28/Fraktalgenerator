#include "imageview.h"

#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
{

}

void ImageView::setImg(QImage i)
{
    this->img = i;
    this->setFixedSize(i.size());
    this->update();

}

ImageView::~ImageView()
{

}

void ImageView::paintEvent(QPaintEvent *event)
{
    auto p = QPainter(this);
    qreal inverseDPR = 1.0 / this->devicePixelRatio();
    p.scale(inverseDPR, inverseDPR);
    p.drawImage(0,0,img);


}
