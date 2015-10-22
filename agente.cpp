#include "agente.h"
#include <QThread>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "mapa.h"
#include <QTimer>

using namespace std;

struct dirYPesos;

agente::agente(int x, int y, int tiempoMov, int id, QWidget* parent) : QWidget(parent){
    x_ = x;
    y_ = y;
    id_ = id;
    padre_ = parent;
    tiempoMov_ = tiempoMov;
    movimientoRestante_ = tiempoMov_;
    activo_ = true;
    rastro_ = false;
    seguir_=false;
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
        bool encontrado = false;
        for(int i=0;i<4;i++){
            if(d.direccion_[i]==0){
                encontrado=true;
            }else if(d.direccion_[i]<5){
                pausar=false;
            }
        }
        if(pausar || encontrado){
            pause();
        }else{
            while (d.direccion_[dir_-1]<1 || d.direccion_[dir_-1]>4) {
                dir_ = rand()%4 + 1;
            }
            dir_--;
            switch (dir_) {
            case arriba:
                y_--;
                break;
            case abajo:
                y_++;
                break;
            case derecha:
                x_++;
                break;
            default:
                x_--;
                break;
            }
            ((mapa*)padre_)->agentePideMovimiento(this,id_,dir_);
        }
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

void agente::setRastro(bool b){
    rastro_ = b;
}

bool agente::getRastro(){
    return rastro_;
}

void agente::setSeguir(bool b){
    seguir_ = b;
}

bool agente::getSeguir(){
    return seguir_;
}

