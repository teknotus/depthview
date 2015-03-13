#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(640, 480); // Only handle dimensions of Depth Camera.
}

void VideoWidget::paintEvent(QPaintEvent *)
{
// Only copying the part that changed worsens performance

//    QRect paintRect = event->rect();
    QPainter painter;
    painter.begin(this);
//    painter.drawImage(paintRect.topLeft(),image.copy(paintRect));
    painter.drawImage(QPoint(0,0),image);
    painter.end();
}

void VideoWidget::setImage(QImage img){
    image = img;
    repaint();
}
