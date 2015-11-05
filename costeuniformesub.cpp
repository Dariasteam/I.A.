#include "costeuniformesub.h"

#include <iostream>

using namespace std;

costeUniformeSub::costeUniformeSub(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem *gPix,
                            QPixmap *lado, mapa *map, mapa *mem, QWidget *parent) :
                            agente(x,y,tiempoMov_,id,gPix,lado,map,mem,parent)
{

}

nodo* costeUniformeSub::expandir(nodo* F){        //profundidad y coste
    actualizarcoordenadas(F->dirLlegar_);
    //imprimir();
    if(F->celda_!=objetivos_.back() && !fin_){
        if(!listaAbierta_.isEmpty()){
            setHijosNodo(F);
            if(!fin_){
                F->completo_=true;
                nodo* K = comprobarCamino(F);
                while(!listaAbierta_.empty() && !listaAbierta_.first()->recorrido_.isEmpty() &&
                     (K==F || esSucesor(F,listaAbierta_.first()->recorrido_.last())) && !fin_){
                        K = expandir(listaAbierta_.first()->recorrido_.at(F->profundidad_+1));
                }
            }
            actualizarcoordenadas(F->dirLlegar_+4);
        }
    }
    return F;
}

void costeUniformeSub::setHijosNodo(nodo* F){
    if(!F->completo_){
        trayectoria* T = listaAbierta_.takeFirst();
        for(int j=0;j<4;j++){
            celda* aux = escanearDireccion(j);
            if(aux!=NULL && aux->tipo_ > -1 && aux->tipo_<5 && !celdaPisada(F,aux)){
                if(aux==objetivos_.back()){
                    cout<<"Encontrado "<<endl;
                    mapaReal_->objetivos_.removeOne(aux);
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
                insertarAbierta(A);
            }
        }
        F->completo_=true;
    }
}
