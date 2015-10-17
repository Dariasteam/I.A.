#include "mapscene.h"


MapScene::MapScene(QObject *parent): QGraphicsScene(parent) {
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    emit mousePos(e->scenePos());
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    emit mousePos(e->scenePos());
}
