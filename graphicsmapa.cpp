#include "graphicsmapa.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

graphicsMapa::graphicsMapa(QObject *parent) : QGraphicsScene(parent)
{
    connect(this,SIGNAL(sendMousePos(QPointF)),parent,SLOT(movioMouse(QPointF)));
    //connect(this,SIGNAL(mouseMoveEvent(QGraphicsSceneMouseEvent*)),
}

QPointF graphicsMapa::GetMousePos()
{
    return mousePos_;
}


void graphicsMapa::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    mousePos_ = e->scenePos();
    qDebug() << e->scenePos().x() << "," << e->scenePos().y() ;
    emit sendMousePos(mousePos_);
}

