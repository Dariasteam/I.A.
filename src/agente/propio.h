#ifndef PROPIO_H
#define PROPIO_H

#include "agente.h"
#include <QList>

class nodo;
class mapa;
class celda;

class nodoC{
public:
    nodoC(nodoC* padre = 0, short dirllegar = -1, celda* c = 0, int profundidad = 0){
        padre_ = padre;
        dirLlegar_ = dirllegar;
        celda_ = c;
        completo_ = false;
        profundidad_ = profundidad;
    }
    nodoC*  padre_;
    short           dirLlegar_;
    celda*          celda_;
    int             profundidad_;
    bool            completo_;
};

struct trayectoriaC{
    QList<nodoC*>   recorrido_;
    short           coste_;
    short           hCoste_ = 0;
};

class propio : public agente{
public:
    propio(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    nodo* expandir(nodo* F);
    void setHijosNodo(nodo* F);
    nodo* intermediario(nodo* F);
    void dijkstra(nodo* I,nodo* F);
    //Funciones centinela
    nodoC* expandirC(nodoC* F);
    void setHijosNodoC(nodoC *F);
    void actualizarcoordenadasC(short);
    void ajustarAbiertaC();
    bool comprobarCerradaC(trayectoriaC*);
    celda* escanearDireccionMem(short);
    bool celdaPisadaC(nodoC*, celda*);
    void insertarAbiertaC(trayectoriaC* A);
    bool esSucesorC(nodoC*, nodoC*);
    nodoC* comprobarCaminoC(nodoC* );
    void imprimirC();
private:
    int             xC_;
    int             yC_;
    QList<trayectoriaC*>              listaAbiertaC_;
    QList<trayectoriaC*>              listaCerradaC_;
    celda*                            objetivoC_;
    nodoC*                            raizC_;
    bool                              finC_;
    QList<short>*                     movimientoC_;
    bool                              atajando_;
};

#endif // PROPIO_H
