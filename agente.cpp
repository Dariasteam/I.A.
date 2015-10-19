#include "agente.h"
#include <QThread>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "mapa.h"
#include <QTimer>

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
    bool caminoAdecuado = false;
    while(!caminoAdecuado){
        dir_ = rand()%4 + 1;                                //1 Arriba, 2 Abajo, 3 Derecha, 4 Izquierda
        caminoAdecuado = ((((mapa*)padre_)->escanearEntorno(x_,y_).direccion_[dir_-1])!=-1);
    }
    cout<<"Iré a un terreno de tipo "<<(((mapa*)padre_)->escanearEntorno(x_,y_).direccion_[dir_-1])<<endl;
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
    ((mapa*)padre_)->agenteFin(id_);
}


QGraphicsPixmapItem* agente::getPix(){
    return pix_;
}
