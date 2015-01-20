#-------------------------------------------------
#
# Project created by QtCreator 2015-01-15T13:51:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = depthview
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videowidget.cpp \
    cameradatafeed.cpp

HEADERS  += mainwindow.h \
    videowidget.h \
    cameradatafeed.h
