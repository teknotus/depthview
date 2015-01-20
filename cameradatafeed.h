#ifndef CAMERADATAFEED_H
#define CAMERADATAFEED_H

#include <QObject>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <QTextStream>
#include <QTimer>
#include <QString>
#include <unistd.h>
//using std::string;

class CameraDataFeed : public QObject
{
    Q_OBJECT
    ~CameraDataFeed();
    QTextStream out;
    QString device;
    int fd;
    __u32 buffercount;
    v4l2_buffer * buffers;
    struct v4l2_requestbuffers reqestBuffers;
    QTimer *timer;
    void printState();
    bool openCamera();
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
//    static const char ** StateStrings = {
//        "OPEN", "FMT", "REQBUFS", "BUFFERS_ARRAY", "QUERYBUF",
//        "MMAP", "QBUF", "STREAM", "TIMER"
//    };
    State state;

public:
    explicit CameraDataFeed(QObject *parent = 0);

signals:
    void newData(void *);

public slots:
    void updateData();
    void startVideo();
    void stopVideo();
    void setCameraDevice(QString);

};

#endif // CAMERADATAFEED_H
