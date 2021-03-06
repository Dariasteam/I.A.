#ifndef costeUniformeSubSUB_H
#define costeUniformeSubSUB_H

#include "agente.h"

class nodo;
class mapa;
class celda;

class costeUniformeSub : public agente{
public:
    costeUniformeSub(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    nodo* expandir(nodo* F);
    void setHijosNodo(nodo* F);
};

#endif // costeUniformeSubSUB_H
