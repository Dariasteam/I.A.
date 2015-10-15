#ifndef GRAPHICSMAPA_H
#define GRAPHICSMAPA_H

#include <QGraphicsScene>
#include <QPointF>


class mapa;


class graphicsMapa : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit graphicsMapa(QObject *parent = 0);
    QPointF mousePos_;
    QPointF GetMousePos();
signals:
    void sendMousePos(QPointF);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);

};

#endif // GRAPHICSMAPA_H
