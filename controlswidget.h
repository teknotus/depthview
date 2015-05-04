#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QListIterator>
#include <QTextStream>
#include <QGroupBox>
#include <linux/videodev2.h>
#include <cameradatafeed.h>
#include <cameracontrolwidget.h>

class ControlsWidget : public QWidget
{
    Q_OBJECT
    QVBoxLayout *controlsLayout;
    QTextStream out;

    QGroupBox * colorGroupBox;
    CameraDataFeed *colorCamera;
    QList<CameraControlWidget *> colorControlWidgets;
    QVBoxLayout * colorControlsLayout;

    QGroupBox * depthGroupBox;
    CameraDataFeed *depthCamera;
    QList<CameraControlWidget *> depthControlWidgets;
    QVBoxLayout * depthControlsLayout;

    QHBoxLayout * buttonLayout;
    QPushButton * defaultsButton;
    QPushButton * refreshButton;

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
    void setControls(CameraDataFeed *,QList<CameraControlWidget *> *,QVBoxLayout *,QList<struct control>);

public:
    explicit ControlsWidget(QWidget *parent = 0);

signals:

public slots:
    void setColorCamera(CameraDataFeed *);
    void setDepthCamera(CameraDataFeed *);
    void setColorControls(QList<struct control>);
    void setDepthControls(QList<struct control>);
    void setDefaults();
    void defaults();
    void refresh();
    void setAccuracy();
    void setInfo();
};

#endif // CONTROLSWIDGET_H
