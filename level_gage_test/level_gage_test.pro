#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T11:56:04
#
#-------------------------------------------------

QT       += core gui
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = level_gage_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial/serial.cpp \
    settingsdialog.cpp \
    qmodbus/qrtumodbus.cpp

HEADERS  += mainwindow.h \
    serial/serial.h \
    settingsdialog.h \
    qmodbus/qabstractmodbus.h \
    qmodbus/qrtumodbus.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
