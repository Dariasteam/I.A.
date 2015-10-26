#include "mapa.h"

class mapa;

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>
#include <thread>
#include <iostream>

using namespace std;

agente::agente(QString texto, int x, int y, double tiempoMov, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, QWidget* parent) : QGroupBox(parent){
    parent_ = parent;
    x_ = x;
    y_ = y;
    lado_ = lado;
    gPix_ = gPix;
    tiempoMov_ = tiempoMov;
    id_ = id;
    valor_=1;
    lay_ = new QGridLayout(this);
    lay_->setSizeConstraint(QLayout::SetMinimumSize);
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
    setCheckable(true);
    connect(this,&QGroupBox::clicked,this,&agente::check);
    connect(checkSeguir_,&QAbstractButton::clicked,((agente*)this),&agente::checkSeguir);
    activo_=false;
    movimientoRestante_=0;
    hilo_ = std::thread(&agente::getColor,this);
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
    if(checkRastro_->isChecked()){
        QPixmap* pix = new QPixmap(gPix_->pixmap());
        pix->fill(color_);
        QGraphicsPixmapItem* aux = ((mapa*)parent_)->pintarPixmap(y_,x_,pix);
        aux->setZValue(1);
        aux->setOpacity(0.2);
    }
    movimiento();
}

void agente::detontante(){
    tiempo_ = new QTimer();
    connect(tiempo_,&QTimer::timeout,this,&agente::animador);
    movimiento();
    tiempo_->start(10);
}

void agente::animador(){
    if(movimientoRestante_>0){
        switch (dir_){
        case arriba:
            gPix_->moveBy(0,-valor_);
            break;
        case abajo:
            gPix_->moveBy(0,valor_);
            break;
        case derecha:
            gPix_->moveBy(valor_,0);
            break;
        default:
            gPix_->moveBy(-valor_,0);
            break;
        }
        if(checkSeguir_->isChecked()){
            ((mapa*)parent_)->seguirAgente(gPix_->x(),gPix_->y());
        }
        movimientoRestante_= movimientoRestante_-valor_;
        if(!movimientoRestante_){
            finMovimiento();
        }
    }
}

void agente::movimiento(){
    if(activo_){
        dirYPesos d = ((mapa*)parent_)->escanearEntorno(x_,y_);
        dir_ = rand()%4 + 1;
        bool pausar = true;
        bool encontrado = false;
        for(int i=0;i<4;i++){
            if(d.direccion_[i]==0){
                encontrado=true;
                gPix_->setPixmap(lado_[i]);
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
            gPix_->setPixmap(lado_[dir_]);
            movimientoRestante_ = tiempoMov_;
            //((mapa*)parent_)->agentePideMovimiento(this,id_,dir_,gPix_,checkSeguir_->isChecked(),checkRastro_->isChecked());
        }
    }

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

void agente::unselectSeguir(){
    checkSeguir_->setChecked(false);
}

void agente::setRastro(bool b){
    checkRastro_->setChecked(b);
}

void agente::checkSeguir(){
    ((mapa*)parent_)->actualizarSeguir(id_);
}

int agente::getX(){
    return x_;
}

int agente::getY(){
    return y_;
}

void agente::setVelocidad(int i){
    tiempo_->start(i);
}
