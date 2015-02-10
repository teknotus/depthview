#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTextStream>
#include <string>
#include <QImage>
#include <QPaintEvent>
//#include <QRect>

class VideoWidget : public QWidget
{
    Q_OBJECT
    QImage image;
    u_int16_t min;
    u_int16_t max;
    u_int16_t mask;

public:
    explicit VideoWidget(QWidget *parent = 0);

signals:

public slots:
    void updateImage(void *);
    void setMin(int);
    void setMax(int);
    void setMask(int);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // VIDEOWIDGET_H
