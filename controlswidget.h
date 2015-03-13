#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <cameradatafeed.h>

class ControlsWidget : public QWidget
{
    Q_OBJECT
    CameraDataFeed *camera;
    QVBoxLayout *controlsLayout;

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
    void setDefaults();
    void setAccuracy();
    void setInfo();
};

#endif // CONTROLSWIDGET_H
