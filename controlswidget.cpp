#include "controlswidget.h"

ControlsWidget::ControlsWidget(QWidget *parent) :
    QWidget(parent)
{
    controlsLayout = new QVBoxLayout();

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

    presetLayout = new QHBoxLayout();
    defaultButton = new QPushButton("defaults");
    accuracyButton = new QPushButton("accuracy");
    infoButton = new QPushButton("information");
    presetLayout->addWidget(defaultButton);
    presetLayout->addWidget(accuracyButton);
    presetLayout->addWidget(infoButton);

    controlsLayout->addLayout(laserLayout);
    controlsLayout->addLayout(ivcamLayout);
    controlsLayout->addLayout(mrtoLayout);
    controlsLayout->addLayout(filterLayout);
    controlsLayout->addLayout(confidenceLayout);
    controlsLayout->addLayout(presetLayout);

    this->setLayout(controlsLayout);

    connect(defaultButton,SIGNAL(clicked()),this,SLOT(setDefaults()));
    connect(accuracyButton,SIGNAL(clicked()),this,SLOT(setAccuracy()));
    connect(infoButton,SIGNAL(clicked()),this,SLOT(setInfo()));
}

void ControlsWidget::setCamera(CameraDataFeed * camera){
    this->camera = camera;
    connect(laserPower,SIGNAL(valueChanged(int)),camera,SLOT(setLaserPower(int)));
    connect(ivcamSetting,SIGNAL(valueChanged(int)),camera,SLOT(setIvcamSetting(int)));
    connect(mrtoSetting,SIGNAL(valueChanged(int)),camera,SLOT(setMrtoSetting(int)));
    connect(filterSetting,SIGNAL(valueChanged(int)),camera,SLOT(setFilterSetting(int)));
    connect(confidenceSetting,SIGNAL(valueChanged(int)),camera,SLOT(setConfidenceSetting(int)));
}

void ControlsWidget::setDefaults(){
    laserPower->setValue(16);
    ivcamSetting->setValue(2);
    mrtoSetting->setValue(0);
    filterSetting->setValue(5);
    confidenceSetting->setValue(6);
}
void ControlsWidget::setAccuracy(){
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
void ControlsWidget::setInfo(){
    laserPower->setValue(16);
    ivcamSetting->setValue(3);
    mrtoSetting->setValue(100);
    filterSetting->setValue(7);
    confidenceSetting->setValue(0);
}
