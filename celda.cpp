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
    setMinimumSize(1,1);
    pix = new QPixmap("../I.A./recursos/suelo.png");
    setPixmap(pix->scaled(this->width(),this->height()));
    setScaledContents(true);
}

celda::~celda(){
    delete pix;
}

void celda::resizeEvent(QResizeEvent* E){
   setPixmap(pix->scaled(this->width(),this->height()));
}

void celda::cambiarTipo(bool b){
    if(b){
        pix->load("../I.A./recursos/muro.png");
        setPixmap(pix->scaled(this->width(),this->height()));
        atravesable_=false;
    }else{
        pix->load("../I.A./recursos/suelo.png");
        setPixmap(pix->scaled(this->width(),this->height()));
        atravesable_=true;
    }
}
