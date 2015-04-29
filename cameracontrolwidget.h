#ifndef CAMERACONTROLWIDGET_H
#define CAMERACONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class CameraControlWidget;
}

class CameraControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraControlWidget(QWidget *parent = 0);
    ~CameraControlWidget();

private:
    Ui::CameraControlWidget *ui;
};

#endif // CAMERACONTROLWIDGET_H
