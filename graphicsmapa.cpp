#include "graphicsmapa.h"
#include "mapa.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class mapa;

graphicsMapa::graphicsMapa(QObject *parent) : QGraphicsScene(parent){
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
    if(E->mimeData()->hasText()){
        E->acceptProposedAction();

    }
}

void graphicsMapa::dropEvent(QGraphicsSceneDragDropEvent* E){
    ((mapa*)parent())->addAgente(E->scenePos());
}


