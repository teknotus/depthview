#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainLayout = new QVBoxLayout();
    vid = new VideoWidget();
    camera = new CameraDataFeed();
    devicePickLayout = new QHBoxLayout();
    deviceLabel = new QLabel("Device");
    devicePathEdit = new QLineEdit();
    deviceLabel->setBuddy(devicePathEdit);
    devicePickLayout->addWidget(deviceLabel);
    devicePickLayout->addWidget(devicePathEdit);
    buttonLayout = new QHBoxLayout();
    startButton = new QPushButton("start");
    stopButton = new QPushButton("stop");
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    mainLayout->addWidget(vid);
    mainLayout->addLayout((devicePickLayout));
    mainLayout->addLayout(buttonLayout);
    centerWidget = new QWidget();
    centerWidget->setLayout(mainLayout);
    setCentralWidget(centerWidget);

    connect(camera, SIGNAL(newData(void*)), vid, SLOT(updateImage(void*)));
    connect(startButton, SIGNAL(clicked()), camera, SLOT(startVideo()));
    connect(stopButton,SIGNAL(clicked()), camera, SLOT(stopVideo()));
    connect(devicePathEdit,SIGNAL(textChanged(QString)),camera,SLOT(setCameraDevice(QString)));

    settings = new QSettings("solsticlipse", "depthview");
    if(settings->contains("depthCameraDevice")){
        devicePathEdit->setText(settings->value("depthCameraDevice").toString());
    }
}

MainWindow::~MainWindow()
{
    settings->setValue("depthCameraDevice",devicePathEdit->text());
    delete settings;
}
