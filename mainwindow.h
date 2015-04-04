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
#include <controlswidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVBoxLayout *mainLayout;
    QVBoxLayout *devicePickLayout;
    QHBoxLayout *colorDevicePickLayout;
    QHBoxLayout *depthDevicePickLayout;
    QHBoxLayout *buttonLayout;
    QWidget *centerWidget;
    VideoWidget *depthVid;
    VideoWidget *irVid;
    CameraDataFeed *camera;
    ControlsWidget *controlsWidget;
    QLabel *colorDeviceLabel;
    QLineEdit *colorDevicePathEdit;
    QLabel *depthDeviceLabel;
    QLineEdit *depthDevicePathEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QSettings * settings;

    QHBoxLayout *snapshotLayout;
    QPushButton *snapshotButton;
    QLabel *snapshotDirEditLabel;
    QLineEdit *snapshotDirEdit;

    QHBoxLayout *fifoRemoteLayout;
    QLabel *fifoRemoteFilenameLabel;
    QLineEdit *fifoRemoteFilenameEdit;

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

};

#endif // MAINWINDOW_H
