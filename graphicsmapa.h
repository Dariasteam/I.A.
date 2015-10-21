#ifndef GRAPHICSMAPA_H
#define GRAPHICSMAPA_H

#include <QGraphicsScene>
#include <QPointF>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QBoxLayout>

class mapa;


class graphicsMapa : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit graphicsMapa(QWidget *parent = 0);
    QPointF mousePos_;
    QBoxLayout*                 layFichas_;
    QPointF GetMousePos();
    void dragMoveEvent(QGraphicsSceneDragDropEvent*);
    void dropEvent(QGraphicsSceneDragDropEvent*);
signals:
    void sendMousePos(QPointF);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
};

#endif // GRAPHICSMAPA_H
