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
    graphicsMapa(QWidget*);
    QPointF                     mousePos_;
    QWidget*                    parent_;
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
