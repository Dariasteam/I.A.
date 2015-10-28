#include "mapa.h"
#include "mainwindow.h"

class MainWindow;

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>
#include <thread>
#include <stdlib.h>
#include <iostream>

using namespace std;

agente::agente(int x, int y, double tiempoMov, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent) : QGroupBox(parent){
    parent_ = parent;
    x_ = x;
    y_ = y;
    lado_ = lado;
    gPix_ = gPix;
    tiempoMov_ = tiempoMov;
    id_ = id;
    valor_=1;
    dir_= 0;
    mapaMem_ = mem;
    mapaReal_ = map;
    lay_ = new QGridLayout(this);
    lay_->setSizeConstraint(QLayout::SetMinimumSize);
    labelBot_.setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));
    QPixmap P("../I.A./recursos/testigo.png");
    color_ = QColor(rand()%255+1,rand()%255+1,rand()%255+1);
    P.fill(color_);
    labelColor_.setPixmap(P);
    lay_->addWidget(&labelBot_,0,0);
    labelText_.setText(("Agente "+QString::fromStdString(std::to_string(id_))));
    lay_->addWidget(&labelText_,0,1);
    lay_->addWidget(&labelColor_,0,2);
    checkRastro_ = new QCheckBox("Rastro",this);
    checkRastro_->setChecked(false);
    checkSeguir_ = new QCheckBox("Seguir",this);
    checkSeguir_->setChecked(false);
    checkMemoria_ = new QCheckBox("Mem",this);
    checkMemoria_->setChecked(false);
    lay_->addWidget(checkRastro_,1,0);
    lay_->addWidget(checkSeguir_,1,1);
    lay_->addWidget(checkMemoria_,1,2);
    setCheckable(true);
    connect(this,&QGroupBox::clicked,this,&agente::check);
    connect(checkSeguir_,&QAbstractButton::clicked,((agente*)this),&agente::setSeguir);
    activo_=false;
    movimientoRestante_=0;
    hilo_ = std::thread(&agente::getX,this);
    hilo_.detach();
}

agente::~agente(){

}

bool agente::terminar(){
    activo_=false;
    cout<<"Es seguro eliminar este agente"<<endl;
    return true;
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

void agente::start(){
    activo_ = true;
    movimiento();
}

void agente::detontante(){
    srand(std::time(0));
    tiempo_ = new QTimer();
    connect(tiempo_,&QTimer::timeout,this,&agente::animador);
    movimiento();
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
        if(activo_ && checkSeguir_->isChecked()){
            mapaReal_->enfocar(gPix_->x(),gPix_->y());
        }
        movimientoRestante_= movimientoRestante_-valor_;
        if(!movimientoRestante_){
            finMovimiento();
        }
    }
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
        QGraphicsPixmapItem* aux = ((mapa*)mapaReal_)->pintarPixmap(y_,x_,pix);
        aux->setZValue(1);
        aux->setOpacity(0.2);
    }
    movimiento();
}

void agente::movimiento(){
    if(activo_){
        short* direccion_;
        direccion_ = mapaReal_->escanearEntorno(x_,y_);
        bool pausar = true;
        bool encontrado = false;
        if(checkMemoria_->isChecked() && mapaMem_!=NULL){
            while(!mapaMem_->mu_.try_lock()){
                cout<<"Esperando desbloqueo del mutex"<<endl;
            }
            mapaMem_->setCelda(y_-1,x_,direccion_[arriba]);
            mapaMem_->setCelda(y_+1,x_,direccion_[abajo]);
            mapaMem_->setCelda(y_,x_+1,direccion_[derecha]);
            mapaMem_->setCelda(y_,x_-1,direccion_[izquierda]);
            mapaMem_->mu_.unlock();
        }
        for(int i=0;i<4;i++){
            if(direccion_[i]==0){
                encontrado=true;
                gPix_->setPixmap(lado_[i]);
            }else if(direccion_[i]<5){
                pausar=false;
            }
        }
        if(pausar || encontrado){
            pause();
        }else{
            dir_ = rand()%4+1;
            while (direccion_[dir_-1]<1 || direccion_[dir_-1]>4) {
                dir_ = rand()%4+1;
            }
            dir_--;
            gPix_->setPixmap(lado_[dir_]);
            movimientoRestante_ = tiempoMov_;
        }
    }
}

bool agente::pause(){
    activo_ = false;
    return true;
}

void agente::setRastro(bool b){
    checkRastro_->setChecked(b);
}

void agente::setSeguir(bool b){
    if(b){
        ((MainWindow*)parent_)->actualizarSeguir(id_);
    }else{
        checkSeguir_->setChecked(false);
    }
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

void agente::setMemoria(bool b){
    checkMemoria_->setChecked(b);
}

