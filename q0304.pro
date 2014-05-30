#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T23:16:19
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = q0304
TEMPLATE = app


SOURCES += main.cpp\
    tower.cpp \
    towerofhanoi.cpp \
    towerview.cpp \
    towersolver.cpp

HEADERS  += \
    tower.h \
    towerofhanoi.h \
    towerview.h \
    towersolver.h

FORMS    += \
    towerofhanoi.ui
