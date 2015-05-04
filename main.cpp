#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 5px; margin-top: 8px;} \
                    QGroupBox::title {  border: 1px solid gray; subcontrol-position: top center; \
                    subcontrol-origin: margin; margin-top: 0px; padding: 0 2px; \
                    border-radius: 3px;}");
    MainWindow w;
    w.show();

    return a.exec();
}
