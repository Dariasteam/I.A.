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
        mainwindow.cpp \
    graphicsmapa.cpp \
    dropbot.cpp \
    agente.cpp \
    mapa.cpp

HEADERS  += mainwindow.h \
    graphicsmapa.h \
    dropbot.h \
    agente.h \
    mapa.h

FORMS    += mainwindow.ui

DISTFILES +=
