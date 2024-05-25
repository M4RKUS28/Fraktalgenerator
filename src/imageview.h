#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QImage>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

#include <QFile>
#include <QImageWriter>



#include <QGesture>
#include <QGestureEvent>
#include <QPinchGesture>
#include <QTapAndHoldGesture>



class ImageView : public QWidget
{
    Q_OBJECT

public:
    ImageView(QWidget * parent = nullptr, bool allowZooming = false);
    ~ImageView();

    void setImage(const QImage &img);
    void setHdpiScale(double multiplikator);

    double getHdpi_scale() const;

    double getHdpi_multiplicator() const;

    const QImage &getImg() const;

    void setControlKeyIsPressed(bool newControlKeyIsPressed);

    void setAllowZooming(bool newAllowZooming);

private:
    QImage img;
    double hdpi_scale;
    double hdpi_multiplicator;
    bool allowZooming;
    bool controlKeyIsPressed;

    bool inPinch;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);


signals:
    void mouseMove(QPoint pos);
    void keyPressed(QKeyEvent *ev);

    void mousePress(QMouseEvent *event, QPoint pos);
    void mouseRelease(QMouseEvent *event, QPoint pos);
    void mouseDoubleClick(QMouseEvent *event, QPoint pos);

    void pinchEvent(bool zoomIn, QPoint posCenter, QPoint posCenterScaled);


    // QObject interface
public:
    bool event(QEvent *event);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void wheelEvent(QWheelEvent *event);
};











#endif // IMAGEVIEW_H
