#include "agente.h"

agente::agente(int x, int y, QGraphicsPixmapItem *pix, QWidget* parent) : QWidget(parent){
    x_ = x;
    y_ = y;
    pix_ = pix;
}


QGraphicsPixmapItem* agente::getPix(){
    return pix_;
}
