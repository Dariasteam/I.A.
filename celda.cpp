#include "celda.h"
#include <QMessageBox>
#include <iostream>
#include <QPixmap>
#include <QSizePolicy>

using namespace std;

celda::celda(int i_, int j_) : QLabel(){
    setMouseTracking(true);
    i=i_;
    j=j_;
    this->setMinimumSize(5,5);
    pix = new QPixmap("../I.A./recursos/suelo.png");
    this->setPixmap(pix->scaled(this->width(),this->height()));
}

void celda::mouseMoveEvent(QMouseEvent* A){
   pix = new QPixmap("../I.A./recursos/muro.png");
   this->setPixmap(pix->scaled(this->width(),this->height()));
}

void celda::resizeEvent(QResizeEvent* E){
   this->setPixmap(pix->scaled(this->width(),this->height()));
}
