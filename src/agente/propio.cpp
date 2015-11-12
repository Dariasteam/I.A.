#include "propio.h"

#include <iostream>

using namespace std;

propio::propio(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem *gPix,
                            QPixmap *lado, mapa *map, mapa *mem, QWidget *parent) :
                            agente(x,y,tiempoMov_,id,gPix,lado,map,mem,parent)
{
    this->checkMemoria_->setChecked(true);
    this->checkMemoria_->setEnabled(false);

    for(int i=0;i<4;i++){
        celda* aux = escanearDireccion(i);
    }
}

nodo* propio::expandir(nodo* F){        //profundidad y coste
    actualizarcoordenadas(F->dirLlegar_);
    cout<<"Estoy en ";
    F->celda_->imprimir();
    cout<<" "<<y_<<" "<<x_<<endl;
    //imprimir();
    ajustarAbierta();
    pasos_++;
    if(F->celda_!=objetivos_.back() && !fin_){
        if(!listaAbierta_.isEmpty()){
            setHijosNodo(F);
            if(!fin_){
                nodo* K = comprobarCamino(F);
                if(K!=F && !esSucesor(F,listaAbierta_.first()->recorrido_.last()) &&F!=raiz_){
                    cout<<"Voy a atajar"<<endl;
                    imprimir();
                    dijkstra(F,K);
                    actualizarcoordenadas(F->dirLlegar_+4,false);
                    return F;
                }
                while(listaAbierta_.count()>0 && listaAbierta_.first()->recorrido_.count()>0
                && K==F || esSucesor(F,listaAbierta_.first()->recorrido_.last()) && !fin_){
                    K = expandir(listaAbierta_.first()->recorrido_.at(F->profundidad_+1));
                }
            }
        }
    }
    actualizarcoordenadas(F->dirLlegar_+4,false);
    cout<<"Retorno"<<endl;
    return F;
}

void propio::setHijosNodo(nodo* F){
    if(!F->completo_){
        trayectoria* T;
        T = listaAbierta_.takeFirst();
        if(!comprobarCerrada(T)){
            listaCerrada_.push_back(T);
        }
        for(int j=0;j<4;j++){
            celda* aux = escanearDireccion(j);
            if(aux!=NULL && aux->tipo_ > -1 && aux->tipo_<5 && !celdaPisada(F,aux)){
                if(aux==objetivos_.back()){
                    cout<<"Encontrado "<<endl;
                    mapaReal_->objetivos_.removeOne(objetivoActual_);
                    trayectoDefinido_.push_back(-1);
                    fin_ = true;
                    break;
                }
                nodo* N = new nodo(F,j,aux,F->profundidad_+1);
                trayectoria* A = new trayectoria;
                (*A).recorrido_ = (*T).recorrido_;
                A->coste_ = T->coste_ + aux->tipo_;
                A->hCoste_ = mapaReal_->getCoste(aux,objetivos_.back());
                A->recorrido_.push_back(N);
                if(!comprobarCerrada(A)){
                    insertarAbierta(A);
                }
            }
        }
        F->completo_=true;
    }
}

nodo* propio::intermediario(nodo *F){
    if(!movimientoC_->isEmpty()){

    }
}

void propio::dijkstra(nodo *I, nodo *F){
    celda* i = I->celda_;
    F = listaAbierta_.first()->recorrido_.last();
    objetivoC_ = F->celda_;
    cout<<"La celda origen es ";
    i->imprimir();
    cout<<endl<<"La celda objetivo es ";
    objetivoC_->imprimir();
    cout<<endl;
    cout<<"Comprobando atajos"<<endl;
    xC_ = x_;
    yC_ = y_;
    cout<<"Y yo estoy en "<<y_<<" "<<x_<<endl;
    movimientoC_ = new QList<short>;
    listaAbiertaC_.clear();
    trayectoriaC* T = new trayectoriaC;
    raizC_ = new nodoC(0,-1,i,0);
    T->recorrido_.push_back(raizC_);
    T->coste_=0;
    T->hCoste_=0;
    listaAbiertaC_.push_back(T);
    finC_ = false;
    expandirC(raizC_);
    while(!movimientoC_->isEmpty()){
        short d = movimientoC_->takeFirst();
        if(d!=-1){
            trayectoDefinido_.push_back(d);
            cout<<trayectoDefinido_.back()<<" ";
        }
    }
    cout<<endl;
    cout<<"Al final de atajar me encuentro en "<<yC_<<","<<xC_<<endl;
}


nodoC* propio::expandirC(nodoC* F){        //profundidad y coste
    //imprimirC();
    actualizarcoordenadasC(F->dirLlegar_);
    ajustarAbiertaC();
    if(F->celda_!=objetivoC_){
        if(!listaAbiertaC_.isEmpty()){
            setHijosNodoC(F);
            if(!finC_){
                nodoC* K = comprobarCaminoC(F);
                while(listaAbiertaC_.count()>0 && listaAbiertaC_.first()->recorrido_.count()>0 &&
                    (K==F || esSucesorC(F,listaAbiertaC_.first()->recorrido_.last())) && !finC_){
                    K = expandirC(listaAbiertaC_.first()->recorrido_.at(F->profundidad_+1));
                }
            }
        }
    }
    if(finC_){
       cout<<"Regresando"<<endl;
       //actualizarcoordenadasC(F->dirLlegar_);
       movimientoC_->push_front(F->dirLlegar_);
    }else{
        if(F!=raizC_){
            actualizarcoordenadasC(F->dirLlegar_+4);
        }
    }
    return F;
}

void propio::setHijosNodoC(nodoC* F){
    if(!F->completo_){
        trayectoriaC* T;
        T = listaAbiertaC_.takeFirst();
        if(!comprobarCerradaC(T)){
            listaCerradaC_.push_back(T);
        }
        for(int j=0;j<4;j++){
            celda* aux = escanearDireccionMem(j);
            if(aux!=NULL && aux->tipo_ > -1 && aux->tipo_<5 && !celdaPisadaC(F,aux)){
                if(aux->x_==objetivoC_->x_ && aux->y_==objetivoC_->y_){
                    cout<<"Atajo encontrado"<<endl;
                    actualizarcoordenadasC(F->dirLlegar_);
                    movimientoC_->push_front(j);
                    finC_ = true;
                    break;
                }
                nodoC* N = new nodoC(F,j,aux,F->profundidad_+1);
                trayectoriaC* A = new trayectoriaC;
                (*A).recorrido_ = (*T).recorrido_;
                A->coste_ = T->coste_ + aux->tipo_;
                A->hCoste_ = mapaMem_->getCoste(aux,objetivoC_);
                A->recorrido_.push_back(N);
                if(!comprobarCerradaC(A)){
                    insertarAbiertaC(A);
                }
            }
        }
        F->completo_=true;
    }
}

celda* propio::escanearDireccionMem(short d){
    if(d==arriba){
        return mapaMem_->getCelda(yC_-1,xC_);
    }else if(d==abajo){
        return mapaMem_->getCelda(yC_+1,xC_);
    }else if(d==derecha){
        return mapaReal_->getCelda(yC_,xC_+1);
    }else if(d==izquierda){
        return mapaReal_->getCelda(yC_,xC_-1);
    }
    return NULL;
}

void propio::actualizarcoordenadasC(short d){
    /*actualiza las coordenadas al punto en el
     *que se encuentre el sentinela*/
    if(d==arriba || d==abajo+4){
        yC_--;
    }else if(d==abajo || d==arriba+4){
        yC_++;
    }else if(d==derecha|| d==izquierda+4){
        xC_++;
    }else if(d==izquierda || d==derecha+4){
        xC_--;
    }
}

void propio::ajustarAbiertaC(){
    /*Elimina trayectorias repetidas de mayor coste de la lista abierta del centinela*/
    int i = 0;
    int n = listaAbiertaC_.count();
    while(i<n){
        nodoC* N = listaAbiertaC_.at(i)->recorrido_.last();
        for(int j=i+1;j<n;j++){
            nodoC* M = listaAbiertaC_.at(j)->recorrido_.last();
            if(M->celda_ == N->celda_){                 //las trayectorias estan ordenadas
                listaAbiertaC_.removeAt(j);
                n--;
            }
        }
        i++;
    }
}

bool propio::comprobarCerradaC(trayectoriaC* T){
    /*retorna true si existe una trayectoria mejor a la propuesta
     * en la lista cerrada centinela y false si no es así*/
    for(int i=0;i<listaCerradaC_.count();i++){
        if(listaCerradaC_.at(i)->coste_ + listaCerradaC_.at(i)->hCoste_ < T->coste_ + T->hCoste_
          && listaCerradaC_.at(i)->recorrido_.last()->celda_==T->recorrido_.last()->celda_){
            return true;
        }
    }
    return false;
}

bool propio::celdaPisadaC(nodoC* N, celda* C){
    /*devuelve true si esta celda ya ha sido pisada
     * en esta trayectoria y false si no es así*/
    if(C  == raizC_->celda_){
        return true;
    }
    while(N!=raizC_){
        if(C == N->celda_){
            return true;
        }
        N = N->padre_;
    }
    return false;
}

void propio::insertarAbiertaC(trayectoriaC* A){
    /*Inserta la nueva trayectoria en la lista abierta centinela
     * en la posición correspondente segun su coste*/
    if(listaAbiertaC_.count()==0){
        listaAbiertaC_.push_back(A);
    }else{
        int i=0;
        while(i<listaAbiertaC_.count() && A->coste_+A->hCoste_ >=listaAbiertaC_.at(i)->coste_ + listaAbiertaC_.at(i)->hCoste_){
            i++;
        }
        listaAbiertaC_.insert(i,A);
    }
}

bool propio::esSucesorC(nodoC* F, nodoC* N){
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

nodoC* propio::comprobarCaminoC(nodoC* N){
    /*Retorna N si estamos en el mismo camino
     * o el nodo comun mas proximo si estamos en otro.
     * En caso de no quedar caminos que comprobar
     * informa de que el problema es irresoluble y
     * devuelve la raiz*/
    if(!listaAbiertaC_.empty()){
        nodoC* aux = N;
        nodoC* list = listaAbiertaC_.first()->recorrido_.last();
        list = list->padre_;
        while(aux!=raizC_){
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
        finC_=true;
        return raizC_;
    }
}

void propio::imprimirC(){
    /*muestra la lista de trayectorias, el contenido de
     *cada una y su coste con el formato:
     *Trayectoria1 (celda1 x,y)[tipo] (celda2 x,y)[tipo] {coste}*/
    for(int i=0;i<listaAbiertaC_.count();i++){
        for(int j=0;j<listaAbiertaC_.at(i)->recorrido_.count();j++){
            cout<<" | ";
            listaAbiertaC_.at(i)->recorrido_.at(j)->celda_->imprimir();
            cout<<" | ";
        }
        cout<<" <- {"<<listaAbiertaC_.at(i)->coste_+listaAbiertaC_.at(i)->hCoste_<<"}"<<endl;
    }
    cout<<endl;
}
