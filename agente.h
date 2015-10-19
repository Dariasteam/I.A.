#ifndef AGENTE_H
#define AGENTE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <thread>
#include <QPixmap>

class agente{
public:
    explicit agente(int x, int y, int id, QGraphicsPixmapItem* pix, QWidget *parent = 0);
    QGraphicsPixmapItem* getPix();
    void movimiento();
    void finMovimiento();
    bool                            finCalculo_;
    int                             dir_;
    int                             movimiento_;
    int                             tiempoMov_;
private:
    int                             id_;
    QGraphicsPixmapItem*            pix_;
    int                             x_;
    int                             y_;
    std::thread                     hilo_;
    QWidget*                        padre_;
};

#endif // AGENTE_H
