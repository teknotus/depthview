#include "cameracontrol.h"

CameraControl::CameraControl(QObject *parent) :
    QObject(parent)
{

}

void CameraControl::initialize(){
//    __u8 * data = (__u8 *)&value;
    struct uvc_xu_control_query xquery;
    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_LEN;
    xquery.size = 2;
    xquery.selector = selector;
    xquery.unit = unit;
    xquery.data = (__u8 *)&size;

//    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
//        perror("UVC_GET_LEN");
//    }
}
