#include "mapscene.h"


MapScene::MapScene(QObject *parent): QGraphicsScene(parent) {
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    emit mousePos(e->scenePos());
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    emit mousePos(e->scenePos());
}

void MapScene::dragMoveEvent(QGraphicsSceneDragDropEvent* ev){
    if(ev->mimeData()->hasText() && ev->mimeData()->text()=="mimeBot"){
        ev->acceptProposedAction();
    }
}

void MapScene::dropEvent(QGraphicsSceneDragDropEvent* event){
    if(event->mimeData() && event->mimeData()->text()=="mimeBot"){
        emit dropAgent(event->scenePos());
    }
}
