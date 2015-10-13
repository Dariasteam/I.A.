#include "celda.h"
#include <QMessageBox>
#include <iostream>
#include <QPixmap>
#include <QSizePolicy>
#include <QMouseEvent>

using namespace std;

celda::celda(int i, int j, QPixmap* suelo, QPixmap* muro, bool tipo, QWidget* parent) : QLabel(parent){
    setMinimumSize(1,1);
    pixSuelo_ = suelo;
    pixMuro_ = muro;
    atravesable_=tipo;
    cambiarTipo(atravesable_);
}

celda::~celda(){}

void celda::resizeEvent(QResizeEvent* E){
    cambiarTipo(atravesable_);
}

void celda::cambiarTipo(bool b){
    if(b){
        setPixmap(pixMuro_->scaled(this->width(),this->height(),Qt::IgnoreAspectRatio, Qt::FastTransformation));
    }else{
        setPixmap(pixSuelo_->scaled(this->width(),this->height(),Qt::IgnoreAspectRatio, Qt::FastTransformation));
    }
}
