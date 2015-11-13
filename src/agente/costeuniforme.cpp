#include "costeuniforme.h"

#include <iostream>

using namespace std;

costeUniforme::costeUniforme(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem *gPix,
                            QPixmap *lado, mapa *map, mapa *mem, QWidget *parent) :
                            agente(x,y,tiempoMov_,id,gPix,lado,map,mem,parent)
{

}

nodo* costeUniforme::expandir(nodo* F){
    actualizarcoordenadas(F->dirLlegar_);
    //imprimir();
    pasos_++;
    if(F->celda_!=objetivos_.back() && !fin_ &&!sinSolucion_){
        if(!listaAbierta_.isEmpty()){
            //EXPANDIR ESTADOS
            setHijosNodo(F);
            if(!fin_){
                F->completo_=true;
                nodo* K = comprobarCamino(F);
                //COMPROBAR POR QUE RAMA CONTINUAR
                while(!listaAbierta_.empty() && !listaAbierta_.first()->recorrido_.isEmpty() &&
                     (K==F || esSucesor(F,listaAbierta_.first()->recorrido_.last())) && !fin_){
                        //SI DEBEMOS CONTINUAR POR ESTA MISMA RAMA AÑADIENDO NUEVOS NODOS HIJOS
                        ramificacion_++;
                        K = expandir(listaAbierta_.first()->recorrido_.at(F->profundidad_+1));
                }
            }
            //SI DEBEMOS REGRESAR A UN NODO ANTERIOR PARA CAMBIAR DE RAMA
            actualizarcoordenadas(F->dirLlegar_+4);
        }
    }
    return F;
}

void costeUniforme::setHijosNodo(nodo* F){
    if(!F->completo_){                              //SOLO REALIZAR ESTA OPERACIÓN UNA VEZ POR NODO
        trayectoria* T = listaAbierta_.takeFirst(); //EXTRAER LA TRAYECTORIA DE MENOR COSTE
        for(int j=0;j<4;j++){                       //PARA CADA UNA DE LAS 4 CELDAS CONTIGUAS POSIBLES
            celda* aux = escanearDireccion(j);
            //SI LA CELDA EXISTE, ES VÁLIDA Y NO HA SIDO PISADA YA EN ESTA RAMA
            if(aux!=NULL && aux->tipo_ > -1 && aux->tipo_<5 && !celdaPisada(F,aux)){
                if(aux==objetivos_.back()){
                    //SI LA CELDA ES EL OBJETIVO
                    cout<<"Encontrado "<<endl;
                    mapaReal_->objetivos_.removeOne(new objetivo {aux, true});
                    trayectoDefinido_.push_back(-1);
                    fin_ = true;
                    break;
                }
                //CREAR NUEVO NODO, CREAR COPIA DE LA TRAYECTORIA T Y AÑADIR EL NUEVO NODO.
                //ACTUALIZANDO EL COSTE
                nodo* N = new nodo(F,j,aux,F->profundidad_+1);
                trayectoria* A = new trayectoria;
                (*A).recorrido_ = (*T).recorrido_;
                A->coste_ = T->coste_ + aux->tipo_;
                A->recorrido_.push_back(N);
                insertarAbierta(A);
            }
        }
        //MARCAR NODO COMO COMPLETO PARA EVITAR QUE SEA REEVALUADO
        F->completo_=true;
    }
}
