#include "agente.h"
#include <QThread>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "mapa.h"
#include <QTimer>

using namespace std;

agente::agente(int x, int y, QGraphicsPixmapItem *pix, QWidget* parent){
    x_ = x;
    y_ = y;
    pix_ = pix;
    padre_ = parent;
    finCalculo_ = false;
    srand(time(NULL));
}

void agente::start(){
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
    bool caminoAdecuado = false;
    while(!caminoAdecuado){
        dir_ = rand()%4 + 1;                                //1 Arriba, 2 Abajo, 3 Derecha, 4 Izquierda
        caminoAdecuado = ((((mapa*)padre_)->escanearEntorno(x_,y_).direccion_[dir_-1])!=-1 &&
                          (((mapa*)padre_)->escanearEntorno(x_,y_).direccion_[dir_-1])<5);
    }
    switch (dir_) {
    case 1:
        y_--;
        break;
    case 2:
        y_++;
        break;
    case 3:
        x_++;
        break;
    default:
        x_--;
        break;
    }
    ((mapa*)padre_)->agentePideMovimiento(this);
}


QGraphicsPixmapItem* agente::getPix(){
    return pix_;
}
