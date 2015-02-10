#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <QObject>
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>

class CameraControl : public QObject
{
    Q_OBJECT
    enum ControlType {
        V4L2_CTRL,
        V4L2_EXT_CTRL,
        UVC_CTRL
    };
    ControlType type;
    __u8 unit;
    __u8 selector;
    __u16 size;
    __u8 min;
    __u8 max;
    __u8 def;
    __u8 current;

public:
    explicit CameraControl(QObject *parent = 0);
    void initialize();

signals:

public slots:

};

#endif // CAMERACONTROL_H
