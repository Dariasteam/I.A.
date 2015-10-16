#include "graphicsmapa.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

graphicsMapa::graphicsMapa(QObject *parent) : QGraphicsScene(parent)
{
    connect(this,SIGNAL(sendMousePos(QPointF)),parent,SLOT(movioMouse(QPointF)));
}

QPointF graphicsMapa::GetMousePos()
{
    return mousePos_;
}


void graphicsMapa::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    mousePos_ = e->scenePos();
    emit sendMousePos(e->scenePos());
}


void graphicsMapa::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendMousePos(event->scenePos());
}

