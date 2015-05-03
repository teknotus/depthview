#include "cameracontrolwidget.h"
#include "ui_cameracontrolwidget.h"

CameraControlWidget::CameraControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraControlWidget)
{
    ui->setupUi(this);
//    connect(this,SIGNAL(setLabel(QString)),ui->controlName,SLOT(setText(QString)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(slideChanged(int)));
    connect(this,SIGNAL(spinUpdate(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(spinChanged(int)));
    connect(this,SIGNAL(slideUpdate(int)),ui->horizontalSlider,SLOT(setValue(int)));
}

CameraControlWidget::~CameraControlWidget()
{
    delete ui;
}

void CameraControlWidget::setCamera(CameraDataFeed * camera){
    this->camera = camera;
}

void CameraControlWidget::setControl(struct control c){
    struct v4l2_queryctrl qc;
    qctrl = qc = c.qctrl;
    QString str((char *)qc.name);
    QTextStream out(stdout);
    __s32 value = camera->getControl(qc.id);
    out << "setLabel: " << str << " value: " << value << endl;
    ui->controlName->setText(str);
    if(qc.type == V4L2_CTRL_TYPE_INTEGER){
        ui->stackedWidget->setCurrentWidget(ui->integerPage);
//        ui->horizontalSlider->setMinimum(qc.minimum);
//        ui->horizontalSlider->setMaximum(qc.maximum);
//        ui->horizontalSlider->setSingleStep(qc.step);
        ui->horizontalSlider->setRange(qc.minimum/qc.step,qc.maximum/qc.step);
        ui->spinBox->setRange(qc.minimum,qc.maximum);
        ui->spinBox->setSingleStep(qc.step);
        ui->horizontalSlider->setValue(value);
        //ui->horizontalSlider->setSingleStep(qc.step);
        //ui->horizontalSlider->setValue(qc.);
    } else if(qc.type == V4L2_CTRL_TYPE_BOOLEAN){
        ui->stackedWidget->setCurrentWidget(ui->boolPage);
        if(value){
            ui->checkBox->setChecked(true);
        } else {
            ui->checkBox->setChecked(false);
        }
    } else if(qc.type == V4L2_CTRL_TYPE_MENU){
        ui->stackedWidget->setCurrentWidget(ui->menuPage);
        out << "menusize: " << c.qmenu.size() << endl;
        QListIterator<struct v4l2_querymenu> qmi(c.qmenu);
        while(qmi.hasNext()){
            struct v4l2_querymenu qm = qmi.next();
            out << "menu item: " << (char *)qm.name << " id: " << qm.id << " index: " << qm.index << endl;
            ui->comboBox->addItem((char *)qm.name,qm.index);
        }
        int comboIdx = ui->comboBox->findData(value);
        out << "comboIdx: " << comboIdx << endl;
        ui->comboBox->setCurrentIndex(comboIdx);
    }
//    ui->checkBox->setText("Disabled");
}

void CameraControlWidget::slideChanged(int slide){
    QTextStream out(stdout);
    out << "slideChanged: " << slide << endl;
    emit spinUpdate(slide * qctrl.step);
}

void CameraControlWidget::spinChanged(int spin){
    emit slideUpdate(spin / qctrl.step);
}
