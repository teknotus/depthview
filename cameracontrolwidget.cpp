#include "cameracontrolwidget.h"
#include "ui_cameracontrolwidget.h"

CameraControlWidget::CameraControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraControlWidget)
{
    ui->setupUi(this);
}

CameraControlWidget::~CameraControlWidget()
{
    delete ui;
}
