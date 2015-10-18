#-------------------------------------------------
#
# Project created by QtCreator 2015-10-07T11:01:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = IA
TEMPLATE = app


SOURCES += main.cpp\
    src/map.cpp \
    src/mapscene.cpp \
    src/mainwindow.cpp \
    src/mapoptions.cpp \
    src/agent/agent.cpp \
    src/agent/sensors.cpp \
    src/map/map.cpp

HEADERS  += src/map.h \
    src/mapscene.h \
    src/mainwindow.h \
    src/mapoptions.h \
    src/agent/agent.hpp \
    src/agent/lrta.hpp \
    src/agent/output.hpp \
    src/agent/sensors.hpp \
    src/map/map.hpp

#FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    tiles.qrc

FORMS += \
    src/mainwindow.ui \
    src/mapoptions.ui
