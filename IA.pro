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
    src/map/map.cpp \
    src/map/mapscene.cpp \
    src/mainwindow.cpp \
    src/options/mapoptions.cpp \
    src/agent/agent.cpp \
    src/options/agentoptions.cpp \
    src/options/dragagent.cpp \
    src/options/agenttoken.cpp \
    src/map/pixmapitem.cpp

HEADERS  += src/map/map.h \
    src/map/mapscene.h \
    src/mainwindow.h \
    src/options/mapoptions.h \
    src/agent/agent.hpp \
    src/options/agentoptions.h \
    src/options/dragagent.h \
    src/options/agenttoken.h \
    src/map/pixmapitem.h

DISTFILES +=

RESOURCES += \
    tiles.qrc

FORMS += \
    src/mainwindow.ui \
    src/options/mapoptions.ui \
    src/options/agentoptions.ui \
    src/options/agenttoken.ui
