#ifndef agente_H
#define agente_H

#include "../mapa/mapa.h"

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

struct celda;

class mapa;

class nodo{
public:
    nodo(nodo* padre = 0, short dirllegar = -1, celda* c = 0, int profundidad = 0){
        padre_ = padre;
        dirLlegar_ = dirllegar;
        celda_ = c;
        completo_ = false;
        profundidad_ = profundidad;
    }
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
    void constructorGui();
    void reiniciar();
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
    int                             valor_;
    int                             xAnimacion_;
    int                             yAnimacion_;
    std::thread                     hiloCalculo_;
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
    mapa*                           mapaReal_;
    mapa*                           mapaMem_;
    QList<short>                    trayectoDefinido_;
public slots:
    void check(bool);
    void animador();
    //ALGORITMO
protected:
    objetivo*                        objetivoActual_;
    int                              pasos_;
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
    virtual nodo* expandir(nodo*) = 0;
    virtual void setHijosNodo(nodo* F) = 0;
    celda* escanearDireccion(short);
    celda* escanearDireccionMem(short);
    void imprimir();
    void insertarAbierta(trayectoria* A);
    nodo* comprobarCamino(nodo*);
    void ajustarAbierta();
};

#endif // agente_H
