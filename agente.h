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

class nodo{
public:
    nodo(nodo* padre = 0, short pcoste = 0, short dirllegar = -1, celda* c = 0){
        padre_ = padre;
        pCoste_ = pcoste;
        dirLlegar_ = dirllegar;
        celda_ = c;
        for(int i=0;i<4;i++){
            hijos_[i]=NULL;
            hCostes_[i] = -1;
            explorado_[i]=false;
        }
    }
    nodo*           hijos_[4];
    bool            explorado_[4];
    nodo*           padre_;
    short           hCostes_[4];
    short           pCoste_;
    short           dirLlegar_;
    celda*          celda_;
};

/*struct nodo{
    celda*      contenido_;
};*/

struct trayectoria{
    QList<nodo*>    recorrido_;
    short           coste_;
};



class agente : public QGroupBox{
public:
    agente(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    ~agente();
    void detontante();
    void start();
    bool terminar();
    void finMovimiento();
    bool pause();
    void setMemoria(bool);
    void setRastro(bool);
    void setSeguir(bool);
    void setVelocidad(int);
    void comprobarMovimientos();
    void writeMem();
    int getX();
    int getY();
private:
    int                             tiempoMov_;
    int                             movimientoRestante_;
    int                             id_;
    int                             x_;
    int                             y_;
    std::thread                     hiloAnimacion_;
    std::thread                     hiloCalculo_;
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
    QList<short>                    trayectoDefinido_;
public slots:
    void check(bool);
    void animador();

    //ALGORITMO
public:

private:
    nodo*                            raiz_;
    nodo*                            actual_;
    QList<trayectoria*>              listaDeTrayectorias_;
    QList<nodo*>                     caminoRecorrido_;
    bool                             regresando_;
    bool                             recuperando_;
    bool                             origen_;
    int                              cuenta_;
    int                              idActual_;
    short                            costazo_;
    bool celdaPisada(trayectoria*, celda*);
    void algoritmo();
    void actualizarcoordenadas(short);
    nodo* expandir(nodo* F);
    void imprimir();
    void insertar(trayectoria* A);
    void recuperar();
    nodo* comprobarCamino();
};

#endif // agente_H
