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

using namespace std;

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
    VideoWidget *colorVid;
    VideoWidget *depthVid;
    VideoWidget *irVid;
    CameraDataFeed *colorCamera;
    CameraDataFeed *depthCamera;
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

    QHBoxLayout *fifoProjOnKeyLayout;
    QLabel *fifoProjOnKeyLabel;
    QLineEdit *fifoProjOnKeyEdit;

    QHBoxLayout *fifoProjOffKeyLayout;
    QLabel *fifoProjOffKeyLabel;
    QLineEdit *fifoProjOffKeyEdit;

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

    QTextStream out;
    QTimer *timer;
    QString fifo_filename;
    int fifo_fd;
    char fifoProjOnKey, fifoProjOffKey;

signals:
    void takeSnap();
    void setLaserPower(int);

public slots:
    void setFifoFilename(QString);
    void setFifoProjOnKey(QString);
    void setFifoProjOffKey(QString);
    void openFifo();
    void closeFifo();
    void checkFifo();
};

#endif // MAINWINDOW_H
