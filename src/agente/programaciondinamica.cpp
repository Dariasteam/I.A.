#include "programaciondinamica.h"

#include <iostream>

using namespace std;

programacionDinamica::programacionDinamica(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem *gPix,
                            QPixmap *lado, mapa *map, mapa *mem, QWidget *parent) :
                            agente(x,y,tiempoMov_,id,gPix,lado,map,mem,parent)
{

}

nodo* programacionDinamica::expandir(nodo* F){        //profundidad y coste
    actualizarcoordenadas(F->dirLlegar_);
    //imprimir();
    pasos_++;
    ajustarAbierta();
    if(F->celda_!=objetivos_.back() && !fin_ &&!sinSolucion_){
        if(!listaAbierta_.isEmpty()){
            setHijosNodo(F);
            if(!fin_){
                nodo* K = comprobarCamino(F);
                while(listaAbierta_.count()>0 && listaAbierta_.first()->recorrido_.count()>0 &&
                    (K==F || esSucesor(F,listaAbierta_.first()->recorrido_.last())) && !fin_){
                    ramificacion_++;
                    K = expandir(listaAbierta_.first()->recorrido_.at(F->profundidad_+1));
                }
            }
            actualizarcoordenadas(F->dirLlegar_+4);
        }
    }
    return F;
}

void programacionDinamica::setHijosNodo(nodo* F){
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
                A->recorrido_.push_back(N);
                if(!comprobarCerrada(A)){
                    insertarAbierta(A);
                }
            }
        }
        F->completo_=true;
    }
}

