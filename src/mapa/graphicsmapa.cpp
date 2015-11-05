#include "graphicsmapa.h"
#include "../mainwindow.h"

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
    if(E->mimeData()->hasText() && (E->mimeData()->text()=="1" ||
                                    E->mimeData()->text()=="2" ||
                                    E->mimeData()->text()=="3" ||
                                    E->mimeData()->text()=="4" )){
        E->acceptProposedAction();
    }
}

void graphicsMapa::dropEvent(QGraphicsSceneDragDropEvent* E){
    if(E->mimeData()->hasText() && (E->mimeData()->text()=="1" ||
                                    E->mimeData()->text()=="2" ||
                                    E->mimeData()->text()=="3" ||
                                    E->mimeData()->text()=="4" )){
         ((MainWindow*)(parent_))->addAgente(E->scenePos(),E->mimeData()->text().toShort());
    }
}


