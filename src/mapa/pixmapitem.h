#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

class PixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit PixmapItem(const QPixmap & pixmap, QGraphicsItem *parent = 0);

signals:

public slots:
};

#endif // PIXMAPITEM_H
