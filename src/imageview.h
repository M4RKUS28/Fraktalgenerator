#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QImage>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>






class ImageView : public QWidget
{
    Q_OBJECT

public:
    ImageView(QWidget * parent = nullptr);
    ~ImageView();

    void setImage(QImage img);
    void setHdpiScale(double multiplikator);

    double getHdpi_scale() const;

    double getHdpi_multiplicator() const;

    const QImage &getImg() const;

private:
    QImage img;
    double hdpi_scale;
    double hdpi_multiplicator;


    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);


signals:
    void mouseMove(QPoint pos);
    void escapePressed();

    // QWidget interface
protected:
//    virtual void mousePressEvent(QMouseEvent *event);
//    virtual void mouseReleaseEvent(QMouseEvent *event);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event);
};











#endif // IMAGEVIEW_H
