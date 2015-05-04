#ifndef CAMERACONTROLWIDGET_H
#define CAMERACONTROLWIDGET_H

#include <QWidget>
#include <QTextStream>
#include <QListIterator>
//#include <string>
#include <linux/videodev2.h>
#include "cameradatafeed.h"

namespace Ui {
class CameraControlWidget;
}

class CameraControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraControlWidget(QWidget *parent = 0);
    ~CameraControlWidget();
    void setControl(struct control);
    void setCamera(CameraDataFeed *);

signals:
    void slideUpdate(int);
    void spinUpdate(int);

public slots:
    void slideChanged(int);
    void spinChanged(int);
    void checkBoxChanged(int);
    void menuChanged(int);
    void setDefault();
    void refresh();

private:
    Ui::CameraControlWidget *ui;
    QTextStream out;
    struct v4l2_queryctrl qctrl;
    CameraDataFeed * camera;
};

#endif // CAMERACONTROLWIDGET_H
