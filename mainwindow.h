#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QSlider>
#include <QButtonGroup>
#include <QRadioButton>
#include <cameradatafeed.h>
#include <videowidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVBoxLayout *mainLayout;
    QHBoxLayout *devicePickLayout;
    QHBoxLayout *buttonLayout;
    QWidget *centerWidget;
    VideoWidget *vid;
    CameraDataFeed *camera;
    QLabel *deviceLabel;
    QLineEdit *devicePathEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QSettings * settings;

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

    QHBoxLayout * maxLayout;
    QLabel * maxLabel;
    QSlider * maxSetting;

    QHBoxLayout * minLayout;
    QLabel * minLabel;
    QSlider * minSetting;

    QHBoxLayout * byteLayout;
    QButtonGroup * byteGroup;
    QRadioButton * bothButton;
    QRadioButton * lowButton;
    QRadioButton * highButton;

    QHBoxLayout * presetLayout;
    QPushButton * defaultButton;
    QPushButton * accuracyButton;
    QPushButton * infoButton;

public slots:
    void setDefaults();
    void setAccuracy();
    void setInfo();
};

#endif // MAINWINDOW_H
