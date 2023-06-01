#include "imageview.h"

#include <QMouseEvent>
#include <QPaintEvent>
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
    qDebug() << "~ImageView()";
}

void ImageView::setImage(const QImage &img)
{
    this->img = img;
    this->setFixedSize(img.size() * hdpi_scale);
    if(parent() == nullptr /* FullscreenView gets no update() when MainWindow is updated!! */) {
        this->update();
    }
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


void ImageView::paintEvent(QPaintEvent * )
{
    QPainter p;
    p.begin(this);
    // wegen hdpi bildschirmen -> hoch auflösendere Bilder!
    p.scale(hdpi_scale, hdpi_scale);

    // zeichne Bild
    p.drawImage(0,0,img);
    p.end();
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    emit ImageView::mouseMove(event->pos() * hdpi_multiplicator);
}

void ImageView::keyPressEvent(QKeyEvent *event)
{
    emit ImageView::keyPressed(event);
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    emit ImageView::mousePress(event, event->pos() * hdpi_multiplicator);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    emit ImageView::mouseRelease(event, event->pos() * hdpi_multiplicator);
}

void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit ImageView::mouseDoubleClick(event, event->pos() * hdpi_multiplicator);
}
