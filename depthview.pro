#-------------------------------------------------
#
# Project created by QtCreator 2015-01-15T13:51:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = depthview
TEMPLATE = app

#INCLUDEPATH += /usr/src/linux-headers-3.17.1-031701/include/ \
#               /usr/src/linux-headers-3.17.1-031701/arch/x86/include/

SOURCES += main.cpp\
        mainwindow.cpp \
    videowidget.cpp \
    cameradatafeed.cpp \
    cameracontrol.cpp

HEADERS  += mainwindow.h \
    videowidget.h \
    cameradatafeed.h \
    cameracontrol.h
