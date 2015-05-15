#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T11:56:04
#
#-------------------------------------------------

QT      += core gui
QT      += serialport
QT      += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = level_gage_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    qmodbus/qrtumodbus.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    qmodbus/qabstractmodbus.h \
    qmodbus/qrtumodbus.h \
    qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
