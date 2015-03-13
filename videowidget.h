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

public:
    explicit VideoWidget(QWidget *parent = 0);

signals:

public slots:
    void setImage(QImage);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // VIDEOWIDGET_H
