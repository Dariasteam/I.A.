#ifndef COSTEUNIFORME_H
#define COSTEUNIFORME_H

#include "agente.h"

class nodo;
class mapa;
class celda;

class costeUniforme : public agente{
public:
    costeUniforme(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    nodo* expandir(nodo* F);
    void setHijosNodo(nodo* F);
};

#endif // COSTEUNIFORME_H
