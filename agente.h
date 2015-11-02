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
    nodo(nodo* padre = 0, short dirllegar = -1, celda* c = 0){
        padre_ = padre;
        dirLlegar_ = dirllegar;
        celda_ = c;
        completo_ = false;
        for(int i=0;i<4;i++){
            hijos_[i]=NULL;
            profundidad_=0;
        }
    }
    nodo*           hijos_[4];
    nodo*           padre_;
    short           dirLlegar_;
    celda*          celda_;
    int             profundidad_;
    bool            completo_;
};

struct trayectoria{
    QList<nodo*>    recorrido_;
    short           coste_;
    short           hCoste_ = 0;
};


class agente : public QGroupBox{
public:
    agente(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    ~agente();
    void detontanteAnimacion();
    void detonanteCalculo();
    void start();
    bool terminar();
    bool pause();
    void setMemoria(bool);
    void pintarRastro();
    void setRastro(bool);
    void setSeguir(bool);
    void setVelocidad(int);
    void comprobarMovimientos();
    void writeMem();
    int getX();
    int getY();
protected:
    int                             tiempoMov_;
    int                             movimientoRestante_;
    int                             id_;
    int                             x_;
    int                             y_;
    int                             xAnimacion_;
    int                             yAnimacion_;
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
protected:
    nodo*                            raiz_;
    bool                             fin_;
    QList<trayectoria*>              listaAbierta_;
    QList<trayectoria*>              listaCerrada_;
    bool                             regresando_;
    QList<celda*>                    objetivos_;
    bool esSucesor(nodo*, nodo*);
    bool comprobarCerrada(trayectoria* T);
    bool celdaPisada(nodo*,celda*);
    void recoger();
    void actualizarcoordenadas(short);
    virtual nodo* expandir(nodo*);
    celda* escanearDireccion(short);
    void imprimir();
    void insertarAbierta(trayectoria* A);
    nodo* comprobarCamino(nodo*);
    void ajustarAbierta();
};

#endif // agente_H
