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

    devicePickLayout = new QVBoxLayout();
    colorDevicePickLayout = new QHBoxLayout();
    colorDeviceLabel = new QLabel("Color Device");
    colorDevicePathEdit = new QLineEdit();
    colorDeviceLabel->setBuddy(colorDevicePathEdit);
    colorDevicePickLayout->addWidget(colorDeviceLabel);
    colorDevicePickLayout->addWidget(colorDevicePathEdit);

    depthDevicePickLayout = new QHBoxLayout();
    depthDeviceLabel = new QLabel("Depth Device");
    depthDevicePathEdit = new QLineEdit();
    depthDeviceLabel->setBuddy(depthDevicePathEdit);
    depthDevicePickLayout->addWidget(depthDeviceLabel);
    depthDevicePickLayout->addWidget(depthDevicePathEdit);
    devicePickLayout->addLayout(colorDevicePickLayout);
    devicePickLayout->addLayout(depthDevicePickLayout);

    snapshotLayout = new QHBoxLayout();
    snapshotDirEditLabel = new QLabel("Snapshot Directory (/tmp/depthview)");
    snapshotDirEdit = new QLineEdit();
    snapshotDirEditLabel->setBuddy(snapshotDirEdit);
    snapshotButton = new QPushButton("save picure");
    snapshotLayout->addWidget(snapshotDirEditLabel);
    snapshotLayout->addWidget(snapshotDirEdit);
    snapshotLayout->addWidget(snapshotButton);

    fifoRemoteLayout = new QHBoxLayout();
    fifoRemoteFilenameLabel = new QLabel("fifo trigger filename (/tmp/depthview/fifo)");
    fifoRemoteFilenameEdit = new QLineEdit();
    fifoRemoteFilenameLabel->setBuddy(fifoRemoteFilenameEdit);
    fifoRemoteLayout->addWidget(fifoRemoteFilenameLabel);
    fifoRemoteLayout->addWidget(fifoRemoteFilenameEdit);

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
    mainLayout->addLayout(fifoRemoteLayout);

    mainLayout->addLayout((devicePickLayout));
    mainLayout->addLayout(buttonLayout);
    centerWidget = new QWidget();
    centerWidget->setLayout(mainLayout);
    setCentralWidget(centerWidget);

    connect(camera,SIGNAL(newDepthImage(QImage)),depthVid,SLOT(setImage(QImage)));
    connect(camera, SIGNAL(newInfraredImage(QImage)), irVid, SLOT(setImage(QImage)));
    connect(startButton, SIGNAL(clicked()), camera, SLOT(startVideo()));
    connect(stopButton,SIGNAL(clicked()), camera, SLOT(stopVideo()));
    connect(colorDevicePathEdit,SIGNAL(textChanged(QString)),camera,SLOT(setCameraDevice(QString)));
    connect(minSetting,SIGNAL(valueChanged(int)),camera,SLOT(setDepthMin(int)));
    connect(maxSetting,SIGNAL(valueChanged(int)),camera,SLOT(setDepthMax(int)));
    connect(byteGroup,SIGNAL(buttonClicked(int)),camera,SLOT(setDepthMask(int)));
    connect(snapshotButton,SIGNAL(clicked()),camera,SLOT(savePicture()));
    connect(snapshotDirEdit,SIGNAL(textChanged(QString)),camera,SLOT(setSnapshotDir(QString)));
    connect(fifoRemoteFilenameEdit,SIGNAL(textChanged(QString)),
            camera,SLOT(setFifoFilename(QString)));

    settings = new QSettings("solsticlipse", "depthview");
    if(settings->contains("colorCameraDevice")){
        colorDevicePathEdit->setText(settings->value("colorCameraDevice").toString());
    }
    if(settings->contains("depthCameraDevice")){
        depthDevicePathEdit->setText(settings->value("depthCameraDevice").toString());
    }
    if(settings->contains("snapshotDirectory")){
        snapshotDirEdit->setText(settings->value("snapshotDirectory").toString());
    }
    if(settings->contains("fifoRemoteFilename")){
        fifoRemoteFilenameEdit->setText(settings->value("fifoRemoteFilename").toString());
    }
}

MainWindow::~MainWindow()
{
    settings->setValue("colorCameraDevice",colorDevicePathEdit->text());
    settings->setValue("depthCameraDevice",depthDevicePathEdit->text());
    settings->setValue("snapshotDirectory",snapshotDirEdit->text());
    settings->setValue("fifoRemoteFilename",fifoRemoteFilenameEdit->text());
    delete settings;
}
