#include "celda.h"
#include <QMessageBox>
#include <iostream>
#include <QPixmap>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QDebug>
using namespace std;

celda::celda(int i, int j, QPixmap* suelo, QPixmap* muro, bool tipo, QWidget* parent) :
    QLabel(parent){
    i_=i;
    j_=j;
    setMinimumSize(1,1);
    pixSuelo_ = suelo;
    pixMuro_ = muro;
    cambiarTipo(tipo);

    qDebug() << "Celda size : " << this->width() << " , " << this->height();

}

celda::~celda(){}

void celda::resizeEvent(QResizeEvent* E){
   cambiarTipo(!atravesable_);                      //llama al ajuste de pixmap manteniendo el valor
}

void celda::cambiarTipo(bool b){
    if(b){
        setPixmap(pixMuro_->scaled(this->width(),this->height()));
        atravesable_=false;
    }else{
        setPixmap(pixSuelo_->scaled(this->width(),this->height()));
        atravesable_=true;
    }
}



bool celda::atravesable(){
    return atravesable_;
}
