#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T23:16:19
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

QMAKE_CXXFLAGS = -Wall -Werror

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = towerofhanoi
TEMPLATE = app

ABOUT_VERSION = v1.1.2

DEFINES *= QT_USE_STRINGBUILDER
DEFINES += "REPOSITORY_URL=\\\"https://bitbucket.org/markshroyer/towerofhanoi\\\""
DEFINES *= "ABOUT_VERSION=\\\"$$ABOUT_VERSION\\\""

win32 {
    DEFINES += "REV_ID=\\\"$$system(commit_id.cmd)\\\""
} else {
    DEFINES += "REV_ID=\\\"$$system(./commit_id.sh)\\\""
}

SOURCES += main.cpp\
    tower.cpp \
    towerofhanoi.cpp \
    towerview.cpp \
    towersolver.cpp \
    stacktracewindow.cpp \
    progresswindow.cpp

HEADERS  += \
    tower.h \
    towerofhanoi.h \
    towerview.h \
    towersolver.h \
    datatypes.h \
    stacktracewindow.h \
    progresswindow.h

FORMS    += \
    towerofhanoi.ui \
    stacktracewindow.ui \
    progresswindow.ui

QMAKE_INFO_PLIST = Info.plist

ICON = towerofhanoi.icns

RESOURCES += \
    towerofhanoi.qrc
