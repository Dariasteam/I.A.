#include "programaciondinamica.h"

#include <iostream>

using namespace std;

/*programacionDinamica::programacionDinamica(int x, int y, double tiempoMov_, int id, QGraphicsPixmapItem *gPix,
                            QPixmap *lado, mapa *map, mapa *mem, QWidget *parent) :
                            agente(x,y,tiempoMov_,id,gPix,lado,map,mem,parent)
{

}

nodo* programacionDinamica::expandir(nodo* F){        //profundidad y coste
    actualizarcoordenadas(F->dirLlegar_);
    imprimir();
    ajustarAbierta();
    if(F->celda_!=objetivos_.back() && !fin_){
        if(!listaAbierta_.isEmpty()){
            setHijosNodo(F);
            if(!fin_){
                nodo* K = comprobarCamino(F);
                bool su = false;
                while(listaAbierta_.count()>0 && listaAbierta_.first()->recorrido_.count()>0 &&
                    (K==F || esSucesor(F,listaAbierta_.first()->recorrido_.last())) && !fin_){
                    su = true;
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

