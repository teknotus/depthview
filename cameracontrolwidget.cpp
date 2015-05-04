#include "cameracontrolwidget.h"
#include "ui_cameracontrolwidget.h"

CameraControlWidget::CameraControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraControlWidget),
    out(stdout)
{
    ui->setupUi(this);
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(slideChanged(int)));
    connect(this,SIGNAL(spinUpdate(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(spinChanged(int)));
    connect(this,SIGNAL(slideUpdate(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(checkBoxChanged(int)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(menuChanged(int)));
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
    __s32 value = camera->getControl(qc.id);
    out << "setLabel: " << str << " value: " << value << endl;
    ui->controlName->setText(str);
    if(qctrl.flags & V4L2_CTRL_FLAG_INACTIVE){
        //ui->stackedWidget->setDisabled(true);
    }
    if(qc.type == V4L2_CTRL_TYPE_INTEGER){
        ui->stackedWidget->setCurrentWidget(ui->integerPage);
        ui->horizontalSlider->setRange(qc.minimum/qc.step,qc.maximum/qc.step);
        ui->spinBox->setRange(qc.minimum,qc.maximum);
        ui->spinBox->setSingleStep(qc.step);
        ui->horizontalSlider->setValue(value);
    } else if(qc.type == V4L2_CTRL_TYPE_BOOLEAN){
        ui->stackedWidget->setCurrentWidget(ui->boolPage);
        if(value){
            ui->checkBox->setChecked(true);
            ui->checkBox->setText("Enabled");
        } else {
            ui->checkBox->setChecked(false);
            ui->checkBox->setText("Disabled");
        }
    } else if(qc.type == V4L2_CTRL_TYPE_MENU){
        ui->stackedWidget->setCurrentWidget(ui->menuPage);
        out << "menusize: " << c.qmenu.size() << endl;
        QListIterator<struct v4l2_querymenu> qmi(c.qmenu);
        while(qmi.hasNext()){
            struct v4l2_querymenu qm = qmi.next();
            out << "menu item: " << (char *)qm.name << " id: " << qm.id << " index: " << qm.index << endl;
            QVariant menuValue((int)qm.index);
//            menuValue.Int = qm.index;
            ui->comboBox->addItem((char *)qm.name,menuValue);
        }
        int comboIdx = ui->comboBox->findData(value);
        out << "comboIdx: " << comboIdx << endl;
        ui->comboBox->setCurrentIndex(comboIdx);
//        ui->comboBox->setModelColumn(1);
    }
}

void CameraControlWidget::slideChanged(int slide){
//    QTextStream out(stdout);
    out << "slideChanged: " << slide << endl;
    int value = slide * qctrl.step;
    camera->setControl(qctrl.id,value);
    emit spinUpdate(value);
}

void CameraControlWidget::spinChanged(int spin){
    emit slideUpdate(spin / qctrl.step);
}

void CameraControlWidget::checkBoxChanged(int checkState){
    __s32 value;
    if(checkState == Qt::Unchecked){
        value = 0;
        ui->checkBox->setText("Disabled");
    } else {
        value = 1;
        ui->checkBox->setText("Enabled");
    }
    camera->setControl(qctrl.id,value);
}

void CameraControlWidget::menuChanged(int index){
    __s32 value = ui->comboBox->itemData(index).toInt();
    out << "menu value: " << value << " index: " << index << endl;
    camera->setControl(qctrl.id,value);
}

void CameraControlWidget::setDefault(){
    camera->setControl(qctrl.id,qctrl.default_value);
}

void CameraControlWidget::refresh(){
    __s32 value = camera->getControl(qctrl.id);
    if(qctrl.type == V4L2_CTRL_TYPE_INTEGER){
        ui->horizontalSlider->setValue(value);
    } else if(qctrl.type == V4L2_CTRL_TYPE_BOOLEAN){
        if(value){
            ui->checkBox->setChecked(true);
            ui->checkBox->setText("Enabled");
        } else {
            ui->checkBox->setChecked(false);
            ui->checkBox->setText("Disabled");
        }
    } else if(qctrl.type == V4L2_CTRL_TYPE_MENU){
        int comboIdx = ui->comboBox->findData(value);
        ui->comboBox->setCurrentIndex(comboIdx);
    }
}
