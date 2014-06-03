#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T23:16:19
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

QMAKE_CXXFLAGS = -Wall -Werror -Wno-unused-parameter

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = towerofhanoi
TEMPLATE = app
DEFINES *= QT_USE_STRINGBUILDER

SOURCES += main.cpp\
    tower.cpp \
    towerofhanoi.cpp \
    towerview.cpp \
    towersolver.cpp \
    stacktracewindow.cpp

HEADERS  += \
    tower.h \
    towerofhanoi.h \
    towerview.h \
    towersolver.h \
    datatypes.h \
    stacktracewindow.h

FORMS    += \
    towerofhanoi.ui \
    stacktracewindow.ui

QMAKE_INFO_PLIST = Info.plist
