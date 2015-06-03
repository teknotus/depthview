#include "controlswidget.h"

ControlsWidget::ControlsWidget(QWidget *parent) :
    QWidget(parent),out(stdout)
{
    controlsLayout = new QVBoxLayout();

    colorGroupBox = new QGroupBox("Color Camera");
    colorControlsLayout = new QVBoxLayout();
    colorGroupBox->setLayout(colorControlsLayout);
    colorGroupBox->setFlat(false);
    controlsLayout->addWidget(colorGroupBox);

    depthGroupBox = new QGroupBox("Depth Camera");
    depthControlsLayout = new QVBoxLayout();
    depthGroupBox->setLayout(depthControlsLayout);
    depthGroupBox->setFlat(true);
    controlsLayout->addWidget(depthGroupBox);

    buttonLayout = new QHBoxLayout();
    defaultsButton = new QPushButton("defaults");
    refreshButton = new QPushButton("refresh");
    buttonLayout->addWidget(defaultsButton);
    buttonLayout->addWidget(refreshButton);
    controlsLayout->addLayout(buttonLayout);

    this->setLayout(controlsLayout);
}

void ControlsWidget::setColorCamera(CameraDataFeed * camera){
    this->colorCamera = camera;
    connect(colorCamera,SIGNAL(newControls(QList<struct control>)),this,SLOT(setColorControls(QList<struct control>)));
}

void ControlsWidget::setDepthCamera(CameraDataFeed * camera){
    this->depthCamera = camera;
    connect(depthCamera,SIGNAL(newControls(QList<struct control>)),this,SLOT(setDepthControls(QList<struct control>)));
}

void ControlsWidget::setColorControls(QList<struct control> controls){
    out << "setColorControls" << endl;
    setControls(colorCamera,&colorControlWidgets,colorControlsLayout,controls);
}

void ControlsWidget::setDepthControls(QList<struct control> controls){
    out << "setDepthControls" << endl;
    setControls(depthCamera,&depthControlWidgets,depthControlsLayout,controls);
}

void ControlsWidget::setControls(CameraDataFeed * camera,
                                 QList<CameraControlWidget *> * widgets,
                                 QVBoxLayout * layout,
                                 QList<struct control> controls){

    out << "in controls: " << controls.size() << " existing controls: " << widgets->size() << endl;
    CameraControlWidget * widge;

    QListIterator<CameraControlWidget *> d(*widgets);
    while(d.hasNext()){
        widge = d.next();
        layout->removeWidget(widge);
        delete widge;
    }
    QListIterator<struct control> i(controls);
    while(i.hasNext()){
        widge = new CameraControlWidget();
        widge->setCamera(camera);
        layout->addWidget(widge);
        widgets->append(widge);
        widge->setControl(i.next());
    }

}
void ControlsWidget::defaults(){
    QListIterator<CameraControlWidget *> c(colorControlWidgets);
    while(c.hasNext()){
        c.next()->setDefault();
    }
    QListIterator<CameraControlWidget *> d(depthControlWidgets);
    while(d.hasNext()){
        d.next()->setDefault();
    }
}

void ControlsWidget::refresh(){
    QListIterator<CameraControlWidget *> c(colorControlWidgets);
    while(c.hasNext()){
        c.next()->refresh();
    }
    QListIterator<CameraControlWidget *> d(depthControlWidgets);
    while(d.hasNext()){
        d.next()->refresh();
    }
}
