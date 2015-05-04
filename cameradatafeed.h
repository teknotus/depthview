#ifndef CAMERADATAFEED_H
#define CAMERADATAFEED_H

#include <QObject>
#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <QTextStream>
#include <QTimer>
#include <QString>
#include <unistd.h>
#include <QImage>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <QList>
//using std::string;

using namespace std;
using namespace cv;

// TODO check for memory leak with qmenu when clearing control struct
struct control {
    struct v4l2_queryctrl qctrl;
    QList<struct v4l2_querymenu> qmenu;
};

class CameraDataFeed : public QObject
{
    Q_OBJECT
    ~CameraDataFeed();
    QTextStream out;
    QString device;
    QString snapshotDir;
    int fd;
    QImage colorImage;
    QImage depthImage;
    QImage infraredImage;
    bool takeSnap;
    u_int16_t depthMin;
    u_int16_t depthMax;
    u_int16_t depthMask;
    u_int32_t fourcc;
    struct v4l2_format v4l2Format;
    __u32 buffercount;
    v4l2_buffer * buffers;
    struct v4l2_requestbuffers reqestBuffers;
    QTimer *timer;
//    QList<struct v4l2_queryctrl> * depthControlList;
    QList<struct control> * controlList;
    void printState();
    bool openCamera();
    void openFifo();
    bool getControls();
    bool setFormat();
    bool reqBuffers();
    bool freeBuffers();
    bool newBufArray();
    bool queryAllBuffers();
    bool mMAP();
    bool qbuf();
    bool startClock();
    bool stopClock();
    bool startStream();
    bool stopStream();
    bool closeCamera();
    bool freeMmap();
    bool freeBufferArray();
    void createImages(void * voidData);
    enum State {
        OPEN =           (1u << 0),
        FMT =            (1u << 1),
        REQBUFS  =       (1u << 2),
        BUFFERS_ARRAY  = (1u << 3),
        QUERYBUF  =      (1u << 4),
        MMAP  =          (1u << 5),
        QBUF  =          (1u << 6),
        STREAM  =        (1u << 7),
        TIMER  =         (1u << 8),
    };
    enum PixelFormat {
        YUYV = 0x56595559,
        INVZ = 0x5a564e49,
        INZI = 0x495a4e49,
        INVR = 0x52564e49,
        INRI = 0x49524e49,
        INVI = 0x49564e49,
        RELI = 0x494c4552
    };

//    static const char ** StateStrings = {
//        "OPEN", "FMT", "REQBUFS", "BUFFERS_ARRAY", "QUERYBUF",
//        "MMAP", "QBUF", "STREAM", "TIMER"
//    };
    State state;

public:
    explicit CameraDataFeed(QObject *parent = 0);
    void setControlUVC(int control, int value);
    __s32 getControl(__u32);
    void setControl(__u32,__s32);

signals:
//    void newData(void *);
    void newColorImage(QImage);
    void newDepthImage(QImage);
    void newInfraredImage(QImage);
//    void newControls(QList<struct v4l2_queryctrl>);
    void newControls(QList<struct control>);

public slots:
    void updateData();
    void startVideo();
    void stopVideo();
    void setCameraDevice(QString);
    void setFourcc(u_int32_t);
    void setLaserPower(int);
    void setIvcamSetting(int);
    void setMrtoSetting(int);
    void setFilterSetting(int);
    void setConfidenceSetting(int);
    void setDepthMin(int);
    void setDepthMax(int);
    void setDepthMask(int);
    void savePicture();
    void setSnapshotDir(QString);
};

#endif // CAMERADATAFEED_H
