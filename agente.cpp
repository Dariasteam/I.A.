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
    cuenta_=0;
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

    nodo* S = new nodo;
    S->contenido_ = mapaReal_->getCelda(y_,x_);
    inicio_ = S;

    trayectoria* T = new trayectoria;
    T->coste_ = 0;
    T->recorrido_.push_back(S);

    regresando_ = false;
    recuperando_ = false;

    caminoActual_.push_back(S);

    lista_.push_back(T);

    hiloAnimacion_ = std::thread(&agente::detontante,this);
    hiloAnimacion_.detach();

    //hiloCalculo_ = std::thread(&agente::algoritmo,this);
    //hiloCalculo_.detach();
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
    algoritmo();
}

void agente::detontante(){
    tiempo_ = new QTimer();
    movimientoRestante_ = tiempoMov_;
    connect(tiempo_,&QTimer::timeout,this,&agente::animador);
}

void agente::animador(){
    if(!trayectoDefinido_.isEmpty()){
        gPix_->setPixmap(lado_[trayectoDefinido_.first()]);
        if(movimientoRestante_>0){
            switch (trayectoDefinido_.first()){
            case arriba:
                gPix_->moveBy(0,-valor_);
                break;
            case abajo:
                gPix_->moveBy(0,valor_);
                break;
            case derecha:
                gPix_->moveBy(valor_,0);
                break;
            case izquierda:
                gPix_->moveBy(-valor_,0);
                break;
            }
            if(activo_ && checkSeguir_->isChecked()){
                mapaReal_->enfocar(gPix_->x(),gPix_->y());
            }
            movimientoRestante_= movimientoRestante_-valor_;
            if(!movimientoRestante_){
                movimientoRestante_ = tiempoMov_;
                finMovimiento();
            }
        }
    }else{
        finMovimiento();
    }
}

void agente::finMovimiento(){
    if(!trayectoDefinido_.isEmpty()){
        switch (trayectoDefinido_.takeFirst()) {
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
    }
    writeMem();
    if(checkRastro_->isChecked()){
        QPixmap* pix = new QPixmap(gPix_->pixmap());
        pix->fill(color_);
        QGraphicsPixmapItem* aux = ((mapa*)mapaReal_)->pintarPixmap(y_,x_,pix);
        aux->setZValue(1);
        aux->setOpacity(0.2);
    }
    if(trayectoDefinido_.isEmpty()){
        regresando_ = false;
        algoritmo();
    }
}

void agente::writeMem(){
    if(checkMemoria_->isChecked() && mapaMem_!=NULL){
        short* direccion_ = mapaReal_->escanearEntorno(x_,y_);
        while(!mapaMem_->mu_.try_lock()){
            cout<<"Esperando desbloqueo del mutex"<<endl;
        }
        mapaMem_->setCelda(y_-1,x_,direccion_[arriba]);
        mapaMem_->setCelda(y_+1,x_,direccion_[abajo]);
        mapaMem_->setCelda(y_,x_+1,direccion_[derecha]);
        mapaMem_->setCelda(y_,x_-1,direccion_[izquierda]);
        mapaMem_->mu_.unlock();
    }
}

void agente::algoritmo(){
    if(activo_){
        expandir();
        if(!lista_.isEmpty()){
            if(comprobarCamino()){
                if(lista_.first()->recorrido_.last()->contenido_->x_ > x_){
                    trayectoDefinido_.push_back(derecha);
                    trayectoRecorrido_.push_back(derecha);
                }else if(lista_.first()->recorrido_.last()->contenido_->x_ < x_){
                    trayectoDefinido_.push_back(izquierda);
                    trayectoRecorrido_.push_back(izquierda);
                }else if(lista_.first()->recorrido_.last()->contenido_->y_ > y_){
                    trayectoDefinido_.push_back(abajo);
                    trayectoRecorrido_.push_back(abajo);
                }else if(lista_.first()->recorrido_.last()->contenido_->y_ < y_){
                    trayectoDefinido_.push_back(arriba);
                    trayectoRecorrido_.push_back(arriba);
                }
                caminoActual_.push_back(lista_.first()->recorrido_.last());
            }else{
                regresar();
            }
        }else{
            //cout<<"La lista está vacía algoritmo"<<endl;
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
    tiempo_->start(5);
}

void agente::setMemoria(bool b){
    checkMemoria_->setChecked(b);
    if(b){
        writeMem();
        mapaMem_->setCelda(y_,x_,mapaReal_->getCelda(y_,x_)->tipo_);
    }
}

void agente::expandir(){
    if(!lista_.isEmpty()){
        trayectoria* T = lista_.takeFirst();
        celda* aux;
        for(int j=0;j<4;j++){
            nodo* N = new nodo;
            if(j==arriba){
                aux = mapaReal_->getCelda(y_-1,x_);
            }else if(j==abajo){
                aux = mapaReal_->getCelda(y_+1,x_);
            }else if(j==derecha){
                aux = mapaReal_->getCelda(y_,x_+1);
            }else{
                aux = mapaReal_->getCelda(y_,x_-1);
            }
            bool insert = true;
            for(int i=0;i<T->recorrido_.count();i++){
                if(T->recorrido_.at(i)->contenido_ == aux){
                    insert = false;
                }
            }
            if((aux->tipo_ > 0 && aux->tipo_ < 5)){
                N->contenido_ = aux;
                trayectoria* A = new trayectoria;
                A->recorrido_ = T->recorrido_;
                A->recorrido_.push_back(N);
                A->coste_ = T->coste_ + aux->tipo_;
                A->id_ = cuenta_;
                if(insert){
                    insertar(A);
                }
                insert = false;
                A=NULL;
                cuenta_++;
            }
        }
        imprimir();
    }else{
        //cout<<"La lista está vacía expandir"<<endl;
    }
}

void agente::imprimir(){
    for(int i=0;i<lista_.count();i++){
        cout<<"("<<lista_.at(i)->id_<<") -> ";
        for(int j=0;j<lista_.at(i)->recorrido_.count();j++){
            cout<<" | ";
            lista_.at(i)->recorrido_.at(j)->contenido_->imprimir();
            cout<<" | ";
        }
        cout<<" <- {"<<lista_.at(i)->coste_<<"}"<<endl;
    }
    cout<<endl<<endl<<"Recorrido hecho"<<endl;
    for(int j=0;j<caminoActual_.count();j++){
        cout<<" | ";
        caminoActual_.at(j)->contenido_->imprimir();
        cout<<" | ";
    }
    cout<<endl;
}

void agente::insertar(trayectoria* A){
    int i=0;
    if(lista_.count()==0){
        lista_.push_back(A);
    }else{
        while(i<lista_.count() && A->coste_ >lista_.at(i)->coste_){
            i++;
        }
        lista_.insert(i,A);
    }
}

void agente::regresar(){
    cout<<"Regresando a casa"<<endl;
    while(!trayectoRecorrido_.isEmpty()){
        regresando_ = true;
        short aux = trayectoRecorrido_.takeLast();
        switch (aux) {
        case arriba:
            trayectoDefinido_.push_back(abajo);
            break;
        case abajo:
            trayectoDefinido_.push_back(arriba);
            break;
        case derecha:
            trayectoDefinido_.push_back(izquierda);
            break;
        case izquierda:
            trayectoDefinido_.push_back(derecha);
            break;
        default:
            break;
        }
    }
}

void agente::recuperar(){

}

bool agente::comprobarCamino(){
    QList<nodo*>* aux = new QList<nodo*>;
    *aux = lista_.first()->recorrido_;
    aux->takeLast();
    if(caminoActual_ != *aux && !regresando_){
        cout<<"Cambio de camino detectado"<<endl;
        return false;
    }else{
        origen_ = false;
        cout<<"Continuamos por el mismo camino"<<endl;
        return true;
    }
}
