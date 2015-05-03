#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QListIterator>
#include <QTextStream>
#include <linux/videodev2.h>
#include <cameradatafeed.h>
#include <cameracontrolwidget.h>

class ControlsWidget : public QWidget
{
    Q_OBJECT
    CameraDataFeed *camera;
    QVBoxLayout *controlsLayout;

    QVBoxLayout * depthControlsLayout;
    QList<CameraControlWidget *> depthControls;
    QTextStream out;

    QHBoxLayout * laserLayout;
    QLabel * laserLabel;
    QSlider * laserPower;

    QHBoxLayout * ivcamLayout;
    QLabel * ivcamLabel;
    QSlider * ivcamSetting;

    QHBoxLayout * mrtoLayout;
    QLabel * mrtoLabel;
    QSlider * mrtoSetting;

    QHBoxLayout * filterLayout;
    QLabel * filterLabel;
    QSlider * filterSetting;

    QHBoxLayout * confidenceLayout;
    QLabel * confidenceLabel;
    QSlider * confidenceSetting;

    QHBoxLayout * presetLayout;
    QPushButton * defaultButton;
    QPushButton * accuracyButton;
    QPushButton * infoButton;

public:
    explicit ControlsWidget(QWidget *parent = 0);

signals:

public slots:
    void setCamera(CameraDataFeed *);
//    void setDepthControls(QList<struct v4l2_queryctrl>);
    void setDepthControls(QList<struct control>);
    void setDefaults();
    void setAccuracy();
    void setInfo();
};

#endif // CONTROLSWIDGET_H
