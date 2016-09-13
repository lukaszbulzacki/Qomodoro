#-------------------------------------------------
#
# Project created by QtCreator 2016-09-08T20:50:01
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 5): error("Project requires Qt 5")

QT       += core gui multimedia
CONFIG   += console c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qomodoro
TEMPLATE = app
INCLUDEPATH = .

SOURCES += main.cpp \
    mainwindow.cpp \
    timememory.cpp \
    timestep.cpp

HEADERS  += mainwindow.h \
    timememory.h \
    timestep.h \
    consts.h

FORMS    += mainwindow.ui

DISTFILES += \
    im-phone-ring.ogg \
    rsync.sh \
    rsync_exclude.txt

RESOURCES += \
    resources.qrc
