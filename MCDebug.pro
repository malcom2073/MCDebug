#-------------------------------------------------
#
# Project created by QtCreator 2018-04-04T11:05:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MCDebug
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


include(../MCIPC/MCIPC.pri)

