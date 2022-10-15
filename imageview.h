#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QImage>
#include <QObject>
#include <QWidget>

class ImageView : public QWidget
{
    Q_OBJECT
public:
    ImageView(QWidget * parent = nullptr);
    void setImg(QImage i);
    ~ImageView();

    QImage img;


    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);
};



#endif // IMAGEVIEW_H
