#include "celda.h"
#include <QMessageBox>
#include <iostream>
#include <QPixmap>
#include <QSizePolicy>
#include <QMouseEvent>

using namespace std;

celda::celda(int i_, int j_) : QLabel(){
    i=i_;
    j=j_;
    setMouseTracking(true);
    this->setMinimumSize(1,1);
    pix = new QPixmap("../I.A./recursos/suelo.png");
    this->setPixmap(pix->scaled(this->width(),this->height()));
}

void celda::resizeEvent(QResizeEvent* E){
   this->setPixmap(pix->scaled(this->width(),this->height()));
}

void celda::cambiarTipo(bool b){
    cout<<"b= "<<b<<endl;
    if(b){
        pix = new QPixmap("../I.A./recursos/muro.png");
        this->setPixmap(pix->scaled(this->width(),this->height()));
        atravesable_=false;
    }else{
        pix = new QPixmap("../I.A./recursos/suelo.png");
        this->setPixmap(pix->scaled(this->width(),this->height()));
        atravesable_=true;
    }
}
