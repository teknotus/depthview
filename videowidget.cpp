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
void VideoWidget::updateImage(void * voidData){
    uchar * data = (uchar *)voidData;
    image = QImage( 640, 480, QImage::Format_ARGB32 );
    for(int i = 0 ; i < 640 ; i++){
        for(int j = 0 ; j < 480 ; j++){
            uchar big = data[j*640*2 + i*2 + 1];
            uchar small = data[j*640*2 + i*2];
            QRgb pix = qRgb( small,255 - (big == 0 ? 255 : big), 0);
            image.setPixel(i,j,pix);
        }
    }
    repaint();
}
