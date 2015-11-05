#ifndef AESTRELLA_H
#define AESTRELLA_H


#include "agente.h"

class nodo;
class mapa;
class celda;

class aEstrella : public agente{
public:
    aEstrella(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    nodo* expandir(nodo* F);
    void setHijosNodo(nodo* F);
};


#endif // AESTRELLA_H
