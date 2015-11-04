#include "costeuniforme.h"

#include <iostream>

using namespace std;

/*costeUniforme::costeUniforme(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem *gPix,
                            QPixmap *lado, mapa *map, mapa *mem, QWidget *parent) :
                            agente(x,y,tiempoMov_,id,gPix,lado,map,mem,parent)
{

}

nodo* costeUniforme::expandir(nodo* F){        //profundidad y coste
    actualizarcoordenadas(F->dirLlegar_);
    //imprimir();
    if(F->celda_!=objetivos_.back() && !fin_){
        if(!listaAbierta_.isEmpty()){
            //EXPANDIR ESTADOS
            setHijosNodo(F);
            //COMPROBAR POR QUE RAMA CONTINUAR
            if(!fin_){
                F->completo_=true;
                nodo* K = comprobarCamino(F);
                while(!listaAbierta_.empty() && !listaAbierta_.first()->recorrido_.isEmpty() &&
                     (K==F || esSucesor(F,listaAbierta_.first()->recorrido_.last())) && !fin_){
                        K = expandir(listaAbierta_.first()->recorrido_.at(F->profundidad_+1));
                }
            }
            actualizarcoordenadas(F->dirLlegar_+4);
            return F;
        }else{

        }
    }
    return NULL;
}*/

