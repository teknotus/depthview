#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainLayout = new QVBoxLayout();
    depthVid = new VideoWidget();
    depthVid->setWindowTitle("Depth");
    depthVid->show();
    irVid = new VideoWidget();
    irVid->setWindowTitle("Infrared");
    irVid->show();
    camera = new CameraDataFeed();
    controlsWidget = new ControlsWidget();
    controlsWidget->setCamera(camera);
    controlsWidget->setWindowTitle("Video Controls");
    controlsWidget->show();

    devicePickLayout = new QHBoxLayout();
    deviceLabel = new QLabel("Device");
    devicePathEdit = new QLineEdit();
    deviceLabel->setBuddy(devicePathEdit);
    devicePickLayout->addWidget(deviceLabel);
    devicePickLayout->addWidget(devicePathEdit);

    snapshotLayout = new QHBoxLayout();
    snapshotDirEditLabel = new QLabel("Snapshot Directory (/tmp)");
    snapshotDirEdit = new QLineEdit();
    snapshotDirEditLabel->setBuddy(snapshotDirEdit);
    snapshotButton = new QPushButton("save picure");
    snapshotLayout->addWidget(snapshotDirEditLabel);
    snapshotLayout->addWidget(snapshotDirEdit);
    snapshotLayout->addWidget(snapshotButton);

    buttonLayout = new QHBoxLayout();
    startButton = new QPushButton("start");
    stopButton = new QPushButton("stop");
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);

    minLayout = new QHBoxLayout();
    minLabel = new QLabel("Min Distance");
    minSetting = new QSlider(Qt::Horizontal);
    minSetting->setMinimum(0);
    minSetting->setMaximum(0xffff);
    minSetting->setValue(0);
    minSetting->setSingleStep(1);
    minSetting->setTickInterval(2048);
    minSetting->setTickPosition(QSlider::TicksBothSides);
    minLayout->addWidget(minLabel);
    minLayout->addWidget(minSetting);

    maxLayout = new QHBoxLayout();
    maxLabel = new QLabel("Max Distance");
    maxSetting = new QSlider(Qt::Horizontal);
    maxSetting->setMinimum(0);
    maxSetting->setMaximum(0xffff);
    maxSetting->setValue(0xffff);
    maxSetting->setSingleStep(1);
    maxSetting->setTickInterval(2048);
    maxSetting->setTickPosition(QSlider::TicksBothSides);
    maxLayout->addWidget(maxLabel);
    maxLayout->addWidget(maxSetting);

    byteLayout = new QHBoxLayout();
    byteGroup = new QButtonGroup();
    bothButton = new QRadioButton("both bytes");
    lowButton = new QRadioButton("low byte");
    highButton = new QRadioButton("high byte");
    byteGroup->addButton(bothButton,0xffff);
    byteGroup->addButton(lowButton,0x00ff);
    byteGroup->addButton(highButton,0xff00);
    bothButton->setChecked(true);
    byteLayout->addWidget(bothButton);
    byteLayout->addWidget(lowButton);
    byteLayout->addWidget(highButton);

    mainLayout->addLayout(minLayout);
    mainLayout->addLayout(maxLayout);
    mainLayout->addLayout(byteLayout);
    mainLayout->addLayout(snapshotLayout);

    mainLayout->addLayout((devicePickLayout));
    mainLayout->addLayout(buttonLayout);
    centerWidget = new QWidget();
    centerWidget->setLayout(mainLayout);
    setCentralWidget(centerWidget);

    connect(camera,SIGNAL(newDepthImage(QImage)),depthVid,SLOT(setImage(QImage)));
    connect(camera, SIGNAL(newInfraredImage(QImage)), irVid, SLOT(setImage(QImage)));
    connect(startButton, SIGNAL(clicked()), camera, SLOT(startVideo()));
    connect(stopButton,SIGNAL(clicked()), camera, SLOT(stopVideo()));
    connect(devicePathEdit,SIGNAL(textChanged(QString)),camera,SLOT(setCameraDevice(QString)));
    connect(minSetting,SIGNAL(valueChanged(int)),camera,SLOT(setDepthMin(int)));
    connect(maxSetting,SIGNAL(valueChanged(int)),camera,SLOT(setDepthMax(int)));
    connect(byteGroup,SIGNAL(buttonClicked(int)),camera,SLOT(setDepthMask(int)));

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
