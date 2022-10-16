#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QImage>
#include <QObject>
#include <QWidget>
#include <QDebug>






class ImageView : public QWidget
{
    Q_OBJECT

public:
    ImageView(QWidget * parent = nullptr);
    ~ImageView();

    void setImage(QImage img);
    void setHdpiScaleEnabled(bool status);
private:
    QImage img;
    double hdpi_scale;
    bool use_hdpi;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:
    void mouseMove(QPoint pos);
};





#endif // IMAGEVIEW_H
