#ifndef AGENTE_H
#define AGENTE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <thread>

class agente{
public:
    explicit agente(int x, int y, int id, QGraphicsPixmapItem* pix, QWidget *parent = 0);
    QGraphicsPixmapItem* getPix();
    void movimiento();
    void finMovimiento();
    bool        finCalculo_;
    int         direccion_;
    int         tiempoMov_;
    int         movimiento_;
private:
    int id_;
    QGraphicsPixmapItem*  pix_;
    int x_;
    int y_;
    std::thread hilo_;
    QWidget*    padre_; 
};

#endif // AGENTE_H
