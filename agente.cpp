#include "mapa.h"

class mapa;

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>
#include <thread>
#include <iostream>

using namespace std;

agente::agente(QString texto, int x, int y, int tiempoMov, int id, QWidget* parent) : QGroupBox(parent){
    parent_ = parent;
    x_ = x;
    y_ = y;
    tiempoMov_ = tiempoMov;
    id_ = id;
    lay_ = new QGridLayout(this);
    lay_->setSizeConstraint(QLayout::SetFixedSize);
    labelBot_.setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));
    QPixmap P("../I.A./recursos/testigo.png");
    color_ = QColor(rand()%255+1,rand()%255+1,rand()%255+1);
    P.fill(color_);
    labelColor_.setPixmap(P);
    lay_->addWidget(&labelBot_,0,0);
    labelText_.setText(texto);
    lay_->addWidget(&labelText_,0,1);
    lay_->addWidget(&labelColor_,0,2);
    checkRastro_ = new QCheckBox("Rastro",this);
    checkRastro_->setChecked(false);
    checkSeguir_ = new QCheckBox("Seguir",this);
    checkSeguir_->setChecked(false);
    lay_->addWidget(checkRastro_,1,0);
    lay_->addWidget(checkSeguir_,1,1);
    lay_->setSizeConstraint(QLayout::SetFixedSize);
    setCheckable(true);
    connect(this,&QGroupBox::clicked,this,&agente::check);
    connect(checkSeguir_,&QAbstractButton::clicked,((agente*)this),&agente::checkSeguir);
    hilo_ = std::thread(&agente::getMovRestante,this);
    hilo_.detach();
}

void agente::mouseDoubleClickEvent(QMouseEvent* E){
    if(E->button() ==Qt::LeftButton){
        if(isChecked()){
            QColorDialog* d = new QColorDialog(this);
            QPixmap P("../I.A./recursos/testigo.png");
            color_ = d->getColor();
            P.fill(color_);
            labelColor_.setPixmap(P);
        }
    }
}

void agente::check(bool b){
    if(b){
        start();
    }else{
        pause();
    }
}

void agente::desactivarSegir(){
    checkSeguir_->setChecked(false);
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
        dirYPesos d = ((mapa*)parent_)->escanearEntorno(x_,y_);
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
            ((mapa*)parent_)->agentePideMovimiento(this,id_,dir_);
        }
    }
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

bool agente::getRastro(){
    return checkRastro_->isChecked();
}

void agente::unselectSeguir(){
    checkSeguir_->setChecked(false);
}

bool agente::getSeguir(){
    return checkSeguir_->isChecked();
}

void agente::setRastro(bool b){
    checkRastro_->setChecked(b);
}

void agente::checkSeguir(){
    ((mapa*)parent_)->actualizarSeguir(id_);
}
