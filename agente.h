#ifndef agente_H
#define agente_H

#include "mapa.h"

#include <QGroupBox>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QColorDialog>
#include <QColor>
#include <QCheckBox>
#include <thread>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QList>

class mapa;

struct celda;

struct nodo{
    QList<celda*>  nodosPasados_;
    int            coste_;
};


class agente : public QGroupBox{
public:
    agente(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    ~agente();
    void detontante();
    void movimiento();
    void start();
    bool terminar();
    void finMovimiento();
    bool pause();
    void setMemoria(bool);
    void setRastro(bool);
    void setSeguir(bool);
    void setVelocidad(int);
    int getX();
    int getY();
private:
    int                             tiempoMov_;
    int                             movimientoRestante_;
    int                             id_;
    int                             x_;
    int                             y_;
    std::thread                     hilo_;
    int                             dir_;
    QGraphicsPixmapItem*            gPix_;
    bool                            activo_;
    QLabel                          labelBot_;
    QLabel                          labelText_;
    QLabel                          labelColor_;
    QGridLayout*                    lay_;
    QWidget*                        parent_;
    QColor                          color_;
    QCheckBox*                      checkRastro_;
    QCheckBox*                      checkSeguir_;
    QCheckBox*                      checkMemoria_;
    QTimer*                         tiempo_;
    QPixmap*                        lado_;
    double                          valor_;
    mapa*                           mapaReal_;
    mapa*                           mapaMem_;
public slots:
    void check(bool);
    void animador();

    //ALGORITMO
public:

private:
    QList<nodo>                     lista_;

};

#endif // agente_H
