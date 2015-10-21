#include "agente.h"
#include <QThread>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "mapa.h"
#include <QTimer>
#include <math.h>
#include <stdio.h>

using namespace std;

struct dirYPesos;

agente::agente(int x, int y, int tiempoMov, int id, QWidget* parent){
    x_ = x;
    y_ = y;
    id_ = id;
    padre_ = parent;
    tiempoMov_ = tiempoMov;
    movimientoRestante_ = tiempoMov_;
}

void agente::start(){
    activo_ = true;
    hilo_ = std::thread(&agente::movimiento,this);
    hilo_.detach();
}

void agente::finMovimiento(){
    movimiento();
}

void agente::movimiento(){
    if(activo_){
        srand(time(NULL));
        movimientoRestante_ = tiempoMov_;
        dirYPesos d = ((mapa*)padre_)->escanearEntorno(x_,y_);
        dir_ = rand()%4 + 1;
        while (d.direccion_[dir_-1]<0 || d.direccion_[dir_-1]>5) {
            dir_ = rand()%4 + 1;
            //1 Arriba, 2 Abajo, 3 Derecha, 4 Izquierda
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
        ((mapa*)padre_)->agentePideMovimiento(this,id_,dir_);
    }
}

void agente::setColor(QColor color){
    color_ = color;
}

int agente::getMovRestante(){
    return movimientoRestante_;
}

int agente::getDir(){
    return dir_;
}

void agente::reducirMov(){
    movimientoRestante_--;
}

int agente::getId(){
    return id_;
}

void agente::pause(){
    activo_ = false;
}

