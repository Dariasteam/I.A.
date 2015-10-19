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
    srand(time(NULL));
    hilo_ = std::thread(&agente::movimiento,this);
    hilo_.detach();
}

void agente::finMovimiento(){
    finCalculo_ = false;
    movimiento();
}

void agente::movimiento(){
    tiempoMov_ = movimiento_;
    finCalculo_ = true;
    direccion_ = rand()%4 + 1;;
    ((mapa*)padre_)->agenteFin(id_);
}


QGraphicsPixmapItem* agente::getPix(){
    return pix_;
}
