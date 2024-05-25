#include "imageview.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

ImageView::ImageView(QWidget *parent, bool allowZooming)
    : QWidget(parent), allowZooming(allowZooming)
{
    // auto
    hdpi_multiplicator = (double)this->devicePixelRatio();
    hdpi_scale = 1.0 / hdpi_multiplicator;
    controlKeyIsPressed = false;
    this->grabGesture(Qt::PinchGesture);
    this->grabGesture(Qt::TapAndHoldGesture);
    inPinch = false;
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

void ImageView::setControlKeyIsPressed(bool newControlKeyIsPressed)
{
    controlKeyIsPressed = newControlKeyIsPressed;
}

void ImageView::setAllowZooming(bool newAllowZooming)
{
    allowZooming = newAllowZooming;
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

void ImageView::wheelEvent(QWheelEvent *event)
{
    if(allowZooming && controlKeyIsPressed) {
        int value = event->angleDelta().y();
        if(value > 0) {
            setHdpiScale(hdpi_multiplicator / 1.02);
            this->update();
        } else if(value < 0) {
            setHdpiScale(hdpi_multiplicator * 1.02);
            this->update();
        }
    } else {
        this->QWidget::wheelEvent(event);
    }
}


bool ImageView::event(QEvent *event)
{
    // qDebug() << event->type();
    if(event->type() == QEvent::Gesture) {
        QGestureEvent* a = reinterpret_cast<QGestureEvent*>(event);
        for( QGesture * g: a->gestures()) {
            if(g->gestureType() == Qt::PinchGesture) {
                QPinchGesture* gesture = static_cast<QPinchGesture*>(g);
                if(g->state() == Qt::GestureState::GestureFinished) {
                    if( gesture->totalScaleFactor() > 3.0) {
                        emit pinchEvent(true, gesture->centerPoint().toPoint(), gesture->centerPoint().toPoint() * hdpi_multiplicator );
                        return true;
                    } else if ( gesture->totalScaleFactor() < 0.333) {
                        emit pinchEvent(false, gesture->centerPoint().toPoint(), gesture->centerPoint().toPoint() * hdpi_multiplicator );
                        return true;

                    }
                    inPinch = false;
                } else if(g->state() == Qt::GestureState::GestureUpdated) {
                    QMouseEvent* move = new QMouseEvent(
                        QEvent::MouseMove,
                        gesture->centerPoint(),
                        Qt::LeftButton,
                        Qt::NoButton,
                        Qt::NoModifier
                        );
                    emit ImageView::mousePress(move, gesture->centerPoint().toPoint() * hdpi_multiplicator);

                } else if(g->state() == Qt::GestureState::GestureStarted) {
                    inPinch = true;
                } else {
                    inPinch = false;
                }
            } else if(g->gestureType() == Qt::TapAndHoldGesture && g->state() == Qt::GestureState::GestureFinished) {
                QTapAndHoldGesture* gesture = static_cast<QTapAndHoldGesture*>(g);
                QMouseEvent* clickEvent = new QMouseEvent(
                    QEvent::MouseButtonPress,
                    gesture->position(),
                    Qt::RightButton,
                    Qt::NoButton,
                    Qt::NoModifier
                    );
                emit ImageView::mousePress(clickEvent, gesture->position().toPoint() * hdpi_multiplicator);
            }
        }
    } else if(event->type() == QEvent::MouseMove && inPinch) {
        return true; // no mousemove while pinch
    }

    return this->QWidget::event(event);
}
