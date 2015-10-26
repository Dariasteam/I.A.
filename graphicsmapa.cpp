#include "graphicsmapa.h"
#include "mainwindow.h"

#include <QGraphicsSceneMouseEvent>

class MainWindow;
class mapa;

graphicsMapa::graphicsMapa(QWidget *parent) : QGraphicsScene(){
    parent_ = parent;
    connect(this,&graphicsMapa::sendMousePos,((MainWindow*)parent_),&MainWindow::movioMouse);
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
         ((MainWindow*)(parent_))->addAgente(E->scenePos());
    }
}


