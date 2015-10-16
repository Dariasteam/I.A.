#ifndef AGENTE_H
#define AGENTE_H

#include <QWidget>
#include <QGraphicsPixmapItem>

class agente : public QWidget{
    Q_OBJECT
public:
    explicit agente(int x, int y, QGraphicsPixmapItem* pix, QWidget *parent = 0);
    QGraphicsPixmapItem* getPix();
private:
    QGraphicsPixmapItem*  pix_;
    int x_;
    int y_;
signals:

public slots:
};

#endif // AGENTE_H
