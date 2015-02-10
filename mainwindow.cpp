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

    laserLayout = new QHBoxLayout();
    laserLabel = new QLabel("Laser Power");
    laserPower = new QSlider(Qt::Horizontal);
    laserPower->setMinimum(0);
    laserPower->setMaximum(16);
    laserPower->setValue(16);
    laserPower->setSingleStep(1);
    laserPower->setTickInterval(1);
    laserPower->setTickPosition(QSlider::TicksBothSides);
    laserLayout->addWidget(laserLabel);
    laserLayout->addWidget(laserPower);

    ivcamLayout = new QHBoxLayout();
    ivcamLabel = new QLabel("IVCAM");
    ivcamSetting = new QSlider(Qt::Horizontal);
    ivcamSetting->setMinimum(1);
    ivcamSetting->setMaximum(3);
    ivcamSetting->setValue(2);
    ivcamSetting->setSingleStep(1);
    ivcamSetting->setTickInterval(1);
    ivcamSetting->setTickPosition(QSlider::TicksBothSides);
    ivcamLayout->addWidget(ivcamLabel);
    ivcamLayout->addWidget(ivcamSetting);

    mrtoLayout = new QHBoxLayout();
    mrtoLabel = new QLabel("Motion Range Trade Off");
    mrtoSetting = new QSlider(Qt::Horizontal);
    mrtoSetting->setMinimum(0);
    mrtoSetting->setMaximum(100);
    mrtoSetting->setValue(0);
    mrtoSetting->setSingleStep(1);
    mrtoSetting->setTickInterval(10);
    mrtoSetting->setTickPosition(QSlider::TicksBothSides);
    mrtoLayout->addWidget(mrtoLabel);
    mrtoLayout->addWidget(mrtoSetting);

    filterLayout = new QHBoxLayout();
    filterLabel = new QLabel("Filter Option");
    filterSetting = new QSlider(Qt::Horizontal);
    filterSetting->setMinimum(0);
    filterSetting->setMaximum(7);
    filterSetting->setValue(5);
    filterSetting->setSingleStep(1);
    filterSetting->setTickInterval(1);
    filterSetting->setTickPosition(QSlider::TicksBothSides);
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterSetting);

    confidenceLayout = new QHBoxLayout();
    confidenceLabel = new QLabel("Confidence Threshold");
    confidenceSetting = new QSlider(Qt::Horizontal);
    confidenceSetting->setMinimum(0);
    confidenceSetting->setMaximum(15);
    confidenceSetting->setValue(6);
    confidenceSetting->setSingleStep(1);
    confidenceSetting->setTickInterval(1);
    confidenceSetting->setTickPosition(QSlider::TicksBothSides);
    confidenceLayout->addWidget(confidenceLabel);
    confidenceLayout->addWidget(confidenceSetting);

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

    presetLayout = new QHBoxLayout();
    defaultButton = new QPushButton("defaults");
    accuracyButton = new QPushButton("accuracy");
    infoButton = new QPushButton("information");
    presetLayout->addWidget(defaultButton);
    presetLayout->addWidget(accuracyButton);
    presetLayout->addWidget(infoButton);

    mainLayout->addWidget(vid);

    mainLayout->addLayout(laserLayout);
    mainLayout->addLayout(ivcamLayout);
    mainLayout->addLayout(mrtoLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addLayout(confidenceLayout);
    mainLayout->addLayout(minLayout);
    mainLayout->addLayout(maxLayout);
    mainLayout->addLayout(byteLayout);
    mainLayout->addLayout(presetLayout);

    mainLayout->addLayout((devicePickLayout));
    mainLayout->addLayout(buttonLayout);
    centerWidget = new QWidget();
    centerWidget->setLayout(mainLayout);
    setCentralWidget(centerWidget);

    connect(camera, SIGNAL(newData(void*)), vid, SLOT(updateImage(void*)));
    connect(startButton, SIGNAL(clicked()), camera, SLOT(startVideo()));
    connect(stopButton,SIGNAL(clicked()), camera, SLOT(stopVideo()));
    connect(devicePathEdit,SIGNAL(textChanged(QString)),camera,SLOT(setCameraDevice(QString)));
    connect(laserPower,SIGNAL(valueChanged(int)),camera,SLOT(setLaserPower(int)));
    connect(ivcamSetting,SIGNAL(valueChanged(int)),camera,SLOT(setIvcamSetting(int)));
    connect(mrtoSetting,SIGNAL(valueChanged(int)),camera,SLOT(setMrtoSetting(int)));
    connect(filterSetting,SIGNAL(valueChanged(int)),camera,SLOT(setFilterSetting(int)));
    connect(confidenceSetting,SIGNAL(valueChanged(int)),camera,SLOT(setConfidenceSetting(int)));
    connect(minSetting,SIGNAL(valueChanged(int)),vid,SLOT(setMin(int)));
    connect(maxSetting,SIGNAL(valueChanged(int)),vid,SLOT(setMax(int)));
    connect(byteGroup,SIGNAL(buttonClicked(int)),vid,SLOT(setMask(int)));
    connect(defaultButton,SIGNAL(clicked()),this,SLOT(setDefaults()));
    connect(accuracyButton,SIGNAL(clicked()),this,SLOT(setAccuracy()));
    connect(infoButton,SIGNAL(clicked()),this,SLOT(setInfo()));

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

void MainWindow::setDefaults(){
    laserPower->setValue(16);
    ivcamSetting->setValue(2);
    mrtoSetting->setValue(0);
    filterSetting->setValue(5);
    confidenceSetting->setValue(6);
    minSetting->setValue(0);
    maxSetting->setValue(0xffff);
    bothButton->setChecked(true);
    QMetaObject::invokeMethod( vid, "setMask", Q_ARG( int, 0xffff ) );
}
void MainWindow::setAccuracy(){
    laserPower->setValue(16);
    ivcamSetting->setValue(3);
//    mrtoSetting->setValue(0);
    filterSetting->setValue(0);
    confidenceSetting->setValue(15);
//    minSetting->setValue(0);
//    maxSetting->setValue(0xffff);
//    bothButton->setChecked(true);
//    QMetaObject::invokeMethod( vid, "setMask", Q_ARG( int, 0xffff ) );
}
void MainWindow::setInfo(){
    laserPower->setValue(16);
    ivcamSetting->setValue(3);
    mrtoSetting->setValue(100);
    filterSetting->setValue(7);
    confidenceSetting->setValue(0);
    minSetting->setValue(0);
    maxSetting->setValue(0xffff);
//    bothButton->setChecked(true);
//    QMetaObject::invokeMethod( vid, "setMask", Q_ARG( int, 0xffff ) );
}
