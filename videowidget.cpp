#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(640, 480); // Only handle dimensions of Depth Camera.
    min = 0;
    max = 0xffff;
    mask = 0xffff;
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
//    uchar * data = (uchar *)voidData;
//    u_int16_t * data = (u_int16_t *)voidData;
    u_int8_t * data = (u_int8_t *)voidData;
    image = QImage( 640, 480, QImage::Format_ARGB32 );
    for(int i = 0 ; i < 640 ; i++){
        for(int j = 0 ; j < 480 ; j++){
//            u_int16_t value = data[j*640 + i];
//            u_int16_t value2;

//            u_int8_t r = data[j*640*3 + i*3];
//            u_int8_t g = data[j*640*3 + i*3 + 1];
//            u_int8_t b = data[j*640*3 + i*3 + 2];

            u_int8_t gray = data[j*640 + i];

            //            u_int16_t value2;
//            value2 = mask & (( value < min || value > max) ? 0 : value);

//            uchar big = data[j*640*2 + i*2 + 1];
//            uchar small = data[j*640*2 + i*2];

//            uchar big = (value2 >> 8) & 0xff;
//            uchar small = value2 & 0xff;
//            QRgb pix = qRgb( small,255 - (big == 0 ? 255 : big), ((value < min) && (value != 0)) ? 255 : 0);
//            QRgb pix = qRgb(r, g, b);
//            QRgb pix = qRgb(r, 0, 0);
//            QRgb pix = qRgb(0, g, 0);
//            QRgb pix = qRgb(0, 0, b);
            QRgb pix = qRgb(0, gray, 0);
            image.setPixel(i,j,pix);
        }
    }
    repaint();
}
void VideoWidget::setMin(int minimum){
    min = (u_int16_t)minimum;
}
void VideoWidget::setMax(int maximum){
    max = (u_int16_t)maximum;
}
void VideoWidget::setMask(int byteMask){
    mask = (u_int16_t)byteMask;
}
