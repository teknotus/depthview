#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
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
};

#endif // MAINWINDOW_H
