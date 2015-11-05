#ifndef PROGRAMACIONDINAMICA_H
#define PROGRAMACIONDINAMICA_H

#include "agente.h"

class nodo;
class mapa;
class celda;

class programacionDinamica : public agente{
public:
    programacionDinamica(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent);
    nodo* expandir(nodo* F);
    void setHijosNodo(nodo* F);
};

#endif // PROGRAMACIONDINAMICA_H
