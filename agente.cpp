#include "agente.h"
#include <QThread>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "mapa.h"
#include <QTimer>

using namespace std;

struct dirYPesos;

agente::agente(int x, int y, int tiempoMov, int id, QWidget* parent){
    x_ = x;
    y_ = y;
    id_ = id;
    padre_ = parent;
    tiempoMov_ = tiempoMov;
    movimientoRestante_ = tiempoMov_;
    activo_ = true;
    srand(time(NULL));
    hilo_ = std::thread(&agente::getDir,this);
    hilo_.detach();
}

void agente::start(){
    activo_ = true;
    movimiento();
}

void agente::finMovimiento(){
    movimiento();
}

void agente::movimiento(){
    if(activo_){
        movimientoRestante_ = tiempoMov_;
        dirYPesos d = ((mapa*)padre_)->escanearEntorno(x_,y_);
        dir_ = rand()%4 + 1;
        bool pausar = true;
        for(int i=0;i<4;i++){
            if(d.direccion_[i]>=0 && d.direccion_[i]<=5){
                pausar=false;
            }
        }
        if(pausar){
            pause();
        }
        while (d.direccion_[dir_-1]<0 || d.direccion_[dir_-1]>4) {
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

bool agente::getActivo(){
    return activo_;
}

QColor agente::getColor(){
    return color_;
}
