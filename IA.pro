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


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/mapa/graphicsmapa.cpp \
    src/dropbot.cpp \
    src/agente/agente.cpp \
    src/mapa/mapa.cpp \
    src/agente/costeuniforme.cpp \
    src/agente/costeuniformesub.cpp \
    src/agente/programaciondinamica.cpp \
    src/agente/aestrella.cpp \
    src/mapa/pixmapitem.cpp

HEADERS  += src/mainwindow.h \
    src/mapa/graphicsmapa.h \
    src/dropbot.h \
    src/agente/agente.h \
    src/mapa/mapa.h \
    src/agente/costeuniforme.h \
    src/agente/costeuniformesub.h \
    src/agente/programaciondinamica.h \
    src/agente/aestrella.h \
    src/mapa/pixmapitem.h

DISTFILES +=

RESOURCES += \
    recursos.qrc
