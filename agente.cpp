#include "agente.h"
#include <QThread>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "mapa.h"
#include <QTimer>

class mapa;

using namespace std;

agente::agente(int x, int y, int id, QGraphicsPixmapItem *pix, QWidget* parent){
    id_ = id;
    x_ = x;
    y_ = y;
    pix_ = pix;
    padre_ = parent;
    finCalculo_ = false;
    hilo_ = std::thread(&agente::movimiento,this);
    hilo_.detach();
}

void agente::finMovimiento(){
    movimiento();
}

void agente::movimiento(){
    finCalculo_ = true;
    direccion_ = rand()%4 + 1;;
    tiempoMov_ = 100;
    ((mapa*)padre_)->agenteFin(id_);
}


QGraphicsPixmapItem* agente::getPix(){
    return pix_;
}
