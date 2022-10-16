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
private:
    QImage img;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
};





#endif // IMAGEVIEW_H
