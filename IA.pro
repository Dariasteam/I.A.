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
    mapa.cpp \
    graphicsmapa.cpp \
    agente.cpp \
    ficha.cpp

HEADERS  += mainwindow.h \
    mapa.h \
    graphicsmapa.h \
    agente.h \
    ficha.h

FORMS    += mainwindow.ui

DISTFILES +=
