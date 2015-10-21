#include "graphicsmapa.h"
#include "mapa.h"
#include "mainwindow.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <iostream>

class MainWindow;
class mapa;

graphicsMapa::graphicsMapa(QWidget *parent) : QGraphicsScene(parent){
    connect(this,SIGNAL(sendMousePos(QPointF)),parent,SLOT(movioMouse(QPointF)));
}

QPointF graphicsMapa::GetMousePos(){
    return mousePos_;
}


void graphicsMapa::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        mousePos_ = e->scenePos();
        emit sendMousePos(e->scenePos());
}


void graphicsMapa::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit sendMousePos(event->scenePos());
}

void graphicsMapa::dragMoveEvent(QGraphicsSceneDragDropEvent* E){
    if(E->mimeData()->hasText() && E->mimeData()->text()=="mimeBot"){
        E->acceptProposedAction();
    }
}

void graphicsMapa::dropEvent(QGraphicsSceneDragDropEvent* E){
    if(E->mimeData()->hasText() && E->mimeData()->text()=="mimeBot"){
         ((mapa*)parent())->addAgente(E->scenePos());
    }
}


