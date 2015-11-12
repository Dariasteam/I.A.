#include "../mapa/mapa.h"
#include "../mainwindow.h"

class MainWindow;

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>
#include <thread>
#include <stdlib.h>
#include <iostream>

using namespace std;

agente::agente(int x, int y, double tiempoMov, int id, QGraphicsPixmapItem* gPix, QPixmap* lado, mapa* map, mapa* mem, QWidget* parent)
    : QGroupBox(parent){
    parent_ = parent;
    x_ = x;
    y_ = y;
    xAnimacion_ = x;
    yAnimacion_ = y;
    lado_ = lado;
    gPix_ = gPix;
    tiempoMov_ = tiempoMov;
    valor_=1;
    id_ = id;
    mapaMem_ = mem;
    mapaReal_ = map;
    regresando_ = false;
    activo_=false;
    movimientoRestante_=0;
    constructorGui();
    detontanteAnimacion();
}

void agente::constructorGui(){
    lay_ = new QGridLayout(this);
    lay_->setSizeConstraint(QLayout::SetMinimumSize);
    labelBot_.setPixmap(lado_[abajo]);
    QPixmap P(":/recursos/testigo.png");

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
}

agente::~agente(){
    delete &hiloCalculo_;
}

void agente::reiniciar(){
    cout<<"Estableciendo variables del algoritmo"<<endl;
    listaAbierta_.clear();
    raiz_ = new nodo();
    pasos_=0;
    raiz_->profundidad_=0;
    raiz_->celda_ = mapaReal_->getCelda(y_,x_);
    raiz_->dirLlegar_=-8;
    fin_ = false;
    trayectoria* T = new trayectoria;
    T->coste_=0;
    T->recorrido_.push_back(raiz_);
    listaAbierta_.push_back(T);
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
            QPixmap P(":/recursos/testigo.png");
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
    int n = mapaReal_->objetivos_.count();
    cout<<"hay "<<n<<" objetivos por encontrar"<<endl;
    while(n>0 && activo_){
        int i=0;
        while(i<n && mapaReal_->objetivos_.at(i)->marcado_){
            i++;
        }
        if(mapaReal_->objetivos_.count()>i){
            objetivoActual_ = mapaReal_->objetivos_.at(i);
            objetivoActual_->marcado_ = true;
            objetivos_.push_back(objetivoActual_->cel_);
            reiniciar();
            expandir(raiz_);
            cout<<"He caminado "<<pasos_<<" pasos "<<endl;
            listaCerrada_.clear();
            trayectoDefinido_.push_back(-1);
            n = mapaReal_->objetivos_.count();
        }
    }
    cout<<"Todos los objetivos han sido encontrados"<<endl;
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
                    mapaReal_->centerOn(gPix_);
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
    mapaReal_->sustituirCelda(objetivos_.first()->y_,objetivos_.first()->x_,suelo);
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

int agente::pasos() {
    return pasos_;
}

QString agente::nombre() {
    return labelText_.text();
}

int agente::pasosCaminoOptimo() {
    return pasosCaminoOptimo_;
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
    /*devuelve true si esta celda ya ha sido pisada
     * en esta trayectoria y false si no es así*/
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
    /*actualiza las coordenadas al punto en el que se encuentre
     *el hilo de cálculo*/
    short e=-1;
    if(d==arriba || d==abajo+4){
        y_--;
        e = arriba;
    }else if(d==abajo || d==arriba+4){
        y_++;
        e = abajo;
    }else if(d==derecha|| d==izquierda+4){
        x_++;
        e = derecha;
    }else if(d==izquierda || d==derecha+4){
        x_--;
        e = izquierda;
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

celda* agente::escanearDireccionMem(short d){
    if(d==arriba){
        return mapaMem_->getCelda(y_-1,x_);
    }else if(d==abajo){
        return mapaMem_->getCelda(y_+1,x_);
    }else if(d==derecha){
        return mapaMem_->getCelda(y_,x_+1);
    }else if(d==izquierda){
        return mapaMem_->getCelda(y_,x_-1);
    }
    return NULL;
}

bool agente::esSucesor(nodo* F, nodo* N){
    /*retorna true si el nodo N es un sucesor del
     *nodo F y false si no es asi. Permite averiguar
     *si la función recursiva debe explorar nuevos caminos
     *o aun no ha llegado al extremo de uno*/
    while(N->profundidad_ >= F->profundidad_){
        if(N==F){
            return true;
        }
        N = N->padre_;
    }
    return false;
}

void agente::imprimir(){
    /*muestra la lista de trayectorias, el contenido de
     *cada una y su coste con el formato:
     *Trayectoria1 (celda1 x,y)[tipo] (celda2 x,y)[tipo] {coste}*/
    for(int i=0;i<listaAbierta_.count();i++){
        for(int j=0;j<listaAbierta_.at(i)->recorrido_.count();j++){
            cout<<" | ";
            listaAbierta_.at(i)->recorrido_.at(j)->celda_->imprimir();
            cout<<" | ";
        }
        cout<<" <- {"<<listaAbierta_.at(i)->coste_+listaAbierta_.at(i)->hCoste_<<"}"<<endl;
    }
    cout<<endl;
}

void agente::insertarAbierta(trayectoria* A){
    /*Inserta la nueva trayectoria en la lista abierta
     * en la posición correspondente segun su coste*/
    if(listaAbierta_.count()==0){
        listaAbierta_.push_back(A);
    }else{
        int i=0;
        while(i<listaAbierta_.count() && A->coste_+A->hCoste_ >=listaAbierta_.at(i)->coste_ + listaAbierta_.at(i)->hCoste_){
            i++;
        }
        listaAbierta_.insert(i,A);
    }
}

void agente::ajustarAbierta(){
    /*Elimina trayectorias repetidas de mayor coste de la lista abierta*/
    int i = 0;
    int n = listaAbierta_.count();
    while(i<n){
        nodo* N = listaAbierta_.at(i)->recorrido_.last();
        for(int j=i+1;j<n;j++){
            nodo* M = listaAbierta_.at(j)->recorrido_.last();
            if(M->celda_ == N->celda_){                 //las trayectorias estan ordenadas
                listaAbierta_.removeAt(j);
                n--;
            }
        }
        i++;
    }
}

bool agente::comprobarCerrada(trayectoria* T){
    /*retorna true si existe una trayectoria mejor a la propuesta
     * en la lista cerrada y false si no es así*/
    for(int i=0;i<listaCerrada_.count();i++){
        if(listaCerrada_.at(i)->coste_ + listaCerrada_.at(i)->hCoste_ < T->coste_ + T->hCoste_
          && listaCerrada_.at(i)->recorrido_.last()->celda_==T->recorrido_.last()->celda_){
            return true;
        }
    }
    return false;
}

nodo* agente::comprobarCamino(nodo* N){
    /*Retorna N si estamos en el mismo camino
     * o el nodo comun mas proximo si estamos en otro.
     * En caso de no quedar caminos que comprobar
     * informa de que el problema es irresoluble y
     * devuelve la raiz*/
    if(!listaAbierta_.empty()){
        nodo* aux = N;
        nodo* list = listaAbierta_.first()->recorrido_.last();
        list = list->padre_;
        while(aux!=raiz_){
            if(aux!=list){          //YA SABEMOS QUE ESTAMOS EN OTRO CAMINO
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
    }else{
        cout<<"El problema no tiene solución"<<endl;
        fin_=true;
        return raiz_;
    }
}
