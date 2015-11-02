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
    xAnimacion_ = x;
    yAnimacion_ = y;
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

    regresando_ = false;
    raiz_ = new nodo();
    raiz_->profundidad_=0;
    raiz_->celda_ = mapaReal_->getCelda(y_,x_);
    raiz_->dirLlegar_=-8;
    fin_ = false;

    objetivos_;


    trayectoria* T = new trayectoria;
    T->coste_=0;
    T->recorrido_.push_back(raiz_);
    listaAbierta_.push_back(T);

    detontanteAnimacion();
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
    mapaMem_->setCelda(y_,x_,mapaReal_->getCelda(y_,x_)->tipo_);
    activo_ = true;
    hiloCalculo_ = std::thread(&agente::detonanteCalculo,this);
    hiloCalculo_.detach();
}

void agente::detontanteAnimacion(){
    tiempo_ = new QTimer();
    movimientoRestante_ = tiempoMov_;
    connect(tiempo_,&QTimer::timeout,this,&agente::animador);
}

void agente::detonanteCalculo(){
    srand(time(NULL));
    int n = mapaReal_->objetivos_.count();
    while(n>0 && activo_){
        fin_ = false;
        int f = rand()%n;
        objetivos_.push_back(mapaReal_->objetivos_.at(f));
        expandir(raiz_);
        listaAbierta_.clear();
        trayectoDefinido_.push_back(-1);
        n = mapaReal_->objetivos_.count();
        while(!trayectoDefinido_.isEmpty()){

        }
    }
}

void agente::animador(){
    if(!trayectoDefinido_.isEmpty()){
        if(trayectoDefinido_.first()==-1){
            if(!regresando_){
                recoger();
            }
            trayectoDefinido_.takeFirst();
            regresando_ = !regresando_;
            pintarRastro();
        }else{
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
            }
            movimientoRestante_= movimientoRestante_-valor_;
            if(!movimientoRestante_){
                switch (trayectoDefinido_.first()){
                case arriba:
                    yAnimacion_--;
                    break;
                case abajo:
                    yAnimacion_++;
                    break;
                case derecha:
                    xAnimacion_++;
                    break;
                case izquierda:
                    xAnimacion_--;
                    break;
                }
                writeMem();
                trayectoDefinido_.takeFirst();
                pintarRastro();
                movimientoRestante_ = tiempoMov_;
            }
        }
    }else{
        tiempo_->stop();
    }
}

void agente::pintarRastro(){
    QPixmap* pix = new QPixmap(gPix_->pixmap());
    if(regresando_){
        pix->fill(QColor(255,255,255));
        QGraphicsPixmapItem* aux = ((mapa*)mapaReal_)->pintarPixmap(yAnimacion_,xAnimacion_,pix);
        aux->setZValue(2);
        aux->setOpacity(1);
    }else if(checkRastro_->isChecked()){
        pix->fill(color_);
        QGraphicsPixmapItem* aux = ((mapa*)mapaReal_)->pintarPixmap(yAnimacion_,xAnimacion_,pix);
        aux->setZValue(1);
        aux->setOpacity(0.2);
    }
}

void agente::writeMem(){
    if(checkMemoria_->isChecked() && mapaMem_!=NULL){
        short* direccion_ = mapaReal_->escanearEntorno(xAnimacion_,yAnimacion_);
        while(!mapaMem_->mu_.try_lock()){
            cout<<"Esperando desbloqueo del mutex"<<endl;
        }
        mapaMem_->setCelda(yAnimacion_-1,xAnimacion_,direccion_[arriba]);
        mapaMem_->setCelda(yAnimacion_+1,xAnimacion_,direccion_[abajo]);
        mapaMem_->setCelda(yAnimacion_,xAnimacion_+1,direccion_[derecha]);
        mapaMem_->setCelda(yAnimacion_,xAnimacion_-1,direccion_[izquierda]);
        mapaMem_->mu_.unlock();
    }
}

void agente::recoger(){
    mapaReal_->setCelda(objetivos_.first()->y_,objetivos_.first()->x_,1);
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

bool agente::celdaPisada(nodo* N, celda* C){
    if(C  == raiz_->celda_){
        return true;
    }
    while(N!=raiz_){
        if(C == N->celda_){
            return true;
        }
        N = N->padre_;
    }
    return false;
}

void agente::actualizarcoordenadas(short d){
    short e=-1;
    if(d==arriba || d==abajo+4){
        y_--;
        e = arriba;
        //cout<<"Mover arriba"<<endl;
    }else if(d==abajo || d==arriba+4){
        y_++;
        e = abajo;
        //cout<<"Mover abajo"<<endl;
    }else if(d==derecha|| d==izquierda+4){
        x_++;
        e = derecha;
        //cout<<"Mover derecha"<<endl;
    }else if(d==izquierda || d==derecha+4){
        x_--;
        e = izquierda;
        //cout<<"Mover izquierda"<<endl;
    }
    if(e>-1 && e<4){
        trayectoDefinido_.push_back(e);
    }
}

celda* agente::escanearDireccion(short d){
    if(d==arriba){
        return mapaReal_->getCelda(y_-1,x_);
    }else if(d==abajo){
        return mapaReal_->getCelda(y_+1,x_);
    }else if(d==derecha){
        return mapaReal_->getCelda(y_,x_+1);
    }else if(d==izquierda){
        return mapaReal_->getCelda(y_,x_-1);
    }
    return NULL;
}

bool agente::esSucesor(nodo* F, nodo* N){
    while(N->profundidad_ >= F->profundidad_){
        if(N==F){
            return true;
        }
        N = N->padre_;
    }
    return false;
}

void agente::imprimir(){
    for(int i=0;i<listaAbierta_.count();i++){
        for(int j=0;j<listaAbierta_.at(i)->recorrido_.count();j++){
            cout<<" | ";
            listaAbierta_.at(i)->recorrido_.at(j)->celda_->imprimir();
            cout<<" | ";
        }
        cout<<" <- {"<<listaAbierta_.at(i)->coste_<<"}"<<endl;
    }
    cout<<endl;
}

void agente::insertar(trayectoria* A){
    if(listaAbierta_.count()==0){
        listaAbierta_.push_back(A);
    }else{
        int i=0;
        while(i<listaAbierta_.count() && A->coste_ >=listaAbierta_.at(i)->coste_){
            i++;
        }
        listaAbierta_.insert(i,A);
    }
}

void agente::ajustarAbierta(){
    int i = 0;
    int n = listaAbierta_.count();
    while(i<n){
        nodo* N = listaAbierta_.at(i)->recorrido_.last();
        for(int j=i+1;j<n;j++){
            nodo* M = listaAbierta_.at(j)->recorrido_.last();
            if(M == N){
                cout<<"Eliminando trayectorias"<<endl;
                if(listaAbierta_.at(i)->coste_ > listaAbierta_.at(j)->coste_){
                    listaAbierta_.removeAt(i);
                }else if(listaAbierta_.at(i)->coste_ < listaAbierta_.at(j)->coste_){
                    listaAbierta_.removeAt(j);
                }
            }
        }
        i++;
    }
}

nodo* agente::expandir(nodo* F){
    return F;
}


nodo* agente::comprobarCamino(nodo* N){
    /*Retorna N si estamos en el mismo camino
     * o el nodo comun mas proximo si estamos en otro*/
    nodo* aux = N;
    nodo* list = listaAbierta_.first()->recorrido_.last();
    list = list->padre_;
    while(aux!=raiz_){
        if(aux!=list){
            while(aux->profundidad_>list->profundidad_){
                aux = aux->padre_;
            }
            while(aux->profundidad_<list->profundidad_){
                list = list->padre_;
            }
            while(aux!=list){
                aux = aux->padre_;
                list = list->padre_;
            }

            return list;
        }
        aux = aux->padre_;
    }
    return N;
}
