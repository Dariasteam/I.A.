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

    raiz_ = new nodo();
    raiz_->celda_ = mapaReal_->getCelda(y_,x_);
    actual_ = raiz_;

    trayectoria* T = new trayectoria;
    T->coste_=0;
    T->recorrido_.push_back(raiz_);
    listaDeTrayectorias_.push_back(T);

    hiloAnimacion_ = std::thread(&agente::detontante,this);
    hiloAnimacion_.join();

    hiloCalculo_ = std::thread(&agente::algoritmo,this);
    hiloCalculo_.join();
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
                trayectoDefinido_.takeFirst();
                finMovimiento();
            }
        }
    }
}

void agente::finMovimiento(){
    writeMem();
    if(checkRastro_->isChecked()){
        QPixmap* pix = new QPixmap(gPix_->pixmap());
        pix->fill(color_);
        QGraphicsPixmapItem* aux = ((mapa*)mapaReal_)->pintarPixmap(y_,x_,pix);
        aux->setZValue(1);
        aux->setOpacity(0.2);
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
        for(int i=0;i<10;i++){
            expandir(raiz_);
            cout<<"Forzado "<<endl;
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
    if(b){
        writeMem();
        mapaMem_->setCelda(y_,x_,mapaReal_->getCelda(y_,x_)->tipo_);
    }
}

bool agente::celdaPisada(trayectoria* t, celda* c){
    for(int i=0;i<t->recorrido_.count();i++){
        if(t->recorrido_.at(i)->celda_ == c){
            return true;
        }
    }
    return false;
}

void agente::actualizarcoordenadas(short d){
    switch (d) {
    case arriba:
        y_--;
        cout<<"Mover arriba"<<endl;
        break;
    case abajo:
        y_++;
        cout<<"Mover abajo"<<endl;
        break;
    case derecha:
        x_++;
        cout<<"Mover derecha"<<endl;
        break;
    case izquierda:
        x_--;
        cout<<"Mover izquierda"<<endl;
        break;
    }
    trayectoDefinido_.push_back(d);
}

nodo* agente::expandir(nodo* F){
    if(F!=NULL){
        imprimir();
        actual_ = F;
        if(!listaDeTrayectorias_.isEmpty()){
            trayectoria* T = listaDeTrayectorias_.takeFirst();
            bool b = true;
            for(int j=0;j<4;j++){
                celda* aux;
                if(j==arriba){
                    aux = mapaReal_->getCelda(y_-1,x_);
                }else if(j==abajo){
                    aux = mapaReal_->getCelda(y_+1,x_);
                }else if(j==derecha){
                    aux = mapaReal_->getCelda(y_,x_+1);
                }else if(j==izquierda){
                    aux = mapaReal_->getCelda(y_,x_-1);
                }
                if(aux->tipo_ > 0 && aux->tipo_<5 && !celdaPisada(T,aux) && F->hijos_[j]==NULL){//sólo introduce los nodos alcanzables
                    nodo* N = new nodo(actual_,aux->tipo_,j,aux);
                    //actual_->hCostes_[j] = aux->tipo_;
                    actual_->hijos_[j] = N;
                    trayectoria* A = new trayectoria;
                    (*A).recorrido_ = (*T).recorrido_;
                    A->coste_ = T->coste_ + aux->tipo_;
                    A->recorrido_.push_back(N);
                    cout<<"Voy a meter ";
                    aux->imprimir();
                    cout<<" y estoy en "<<x_<<" , "<<y_<<endl;
                    insertar(A);
                    b=true;
                }
            }
            if(!b){
                insertar(T);
            }else{
                delete T;
            }
            if(comprobarCamino()!=NULL){
                cout<<"No hay salida"<<endl;
                return actual_->padre_;
            }
            nodo* N = listaDeTrayectorias_.first()->recorrido_.last();
            short d = N->dirLlegar_;
            costazo_ = listaDeTrayectorias_.first()->coste_;
            caminoRecorrido_.push_back(listaDeTrayectorias_.first()->recorrido_.last());
            actualizarcoordenadas(d);
            expandir(N);
            switch (d) {
            case arriba:
                d =abajo;
                break;
            case abajo:
                d = arriba;
                break;
            case derecha:
                d = izquierda;
                break;
            case izquierda:
                d  = derecha;
                break;
            }
            actualizarcoordenadas(d);
            caminoRecorrido_.takeLast();
            return F;
        }
    }else{
        cout<<"Un nulo"<<endl;
    }
}

void agente::imprimir(){
    for(int i=0;i<listaDeTrayectorias_.count();i++){
        for(int j=0;j<listaDeTrayectorias_.at(i)->recorrido_.count();j++){
            cout<<" | ";
            listaDeTrayectorias_.at(i)->recorrido_.at(j)->celda_->imprimir();
            cout<<" | ";
        }
        cout<<" <- {"<<listaDeTrayectorias_.at(i)->coste_<<"}"<<endl;
    }
    cout<<endl;
}

void agente::insertar(trayectoria* A){
    int i=0;
    if(listaDeTrayectorias_.count()==0){
        listaDeTrayectorias_.push_back(A);
    }else{
        while(i<listaDeTrayectorias_.count() && A->coste_ >listaDeTrayectorias_.at(i)->coste_){
            i++;
        }
        listaDeTrayectorias_.insert(i,A);
    }
}

void agente::recuperar(){

}

nodo* agente::comprobarCamino(){
    if(!caminoRecorrido_.isEmpty()){
        QList<nodo*>* aux= new QList<nodo*>;
        QList<nodo*>* aux2 = new QList<nodo*>;
        QList<nodo*>* aux3 = new QList<nodo*>;
        QList<nodo*>* aux4 = new QList<nodo*>;

        for(int i=0;i<4;i++){
            *aux = listaDeTrayectorias_.at(i)->recorrido_;
            aux->takeLast();
            if(caminoRecorrido_ != *aux){
                return aux->first();
            }else{
                return NULL;
            }
        }
    }
    return NULL;
}
