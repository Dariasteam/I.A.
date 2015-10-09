#include "mapa.h"
#include <iostream>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QGridLayout>
#include <QProgressBar>
#include <QList>
#include <QBoxLayout>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

mapa::mapa(int filas, int columnas, QProgressBar* barra_,int factor, QWidget* parent) : QWidget(parent){
    f_=filas;
    c_=columnas;
    srand(time(NULL));
    int aleatoriedad;
    layMapa_ = new QGridLayout();
    layMapa_->setSpacing(0);
    if(filas<35 && columnas<35){
        pixSuelo_ = new QPixmap("../I.A./recursos/suelo.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muro.png");
    }else{
        pixSuelo_ = new QPixmap("../I.A./recursos/sueloLow.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muroLow.png");
    }
    barra_->show();
    barra_->setMaximum(columnas*filas);
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    int barI=1;
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            aleatoriedad = rand()%100+ 1;
            if(aleatoriedad<=factor){
                layMapa_->addWidget (new celda(i,j,pixSuelo_,pixMuro_,true,this),i,j);
            }else{
                layMapa_->addWidget (new celda(i,j,pixSuelo_,pixMuro_,false,this),i,j);
            }
            emit actualizarBarra(barI);
            barI++;
        }
    }
    barra_->hide();
    setLayout(layMapa_);
}

void mapa::mousePressEvent(QMouseEvent* E){
    if(E->button()==Qt::LeftButton){
        pintar_=true;
    }else if(E->button()==Qt::RightButton){
        pintar_=false;
    }
}

void mapa::resizeEvent(QResizeEvent* ){
   layMapa_->update();
}

void mapa::mouseMoveEvent(QMouseEvent* ){
    QSize celdaSz = layMapa_->itemAtPosition(0,0)->widget()->size();
    QPoint cursor = this->mapFromGlobal(QCursor::pos());
    cout<<"Cursor: "<<cursor.x()<<","<<cursor.y()<<endl;
    QMargins margen = layMapa_->contentsMargins();
    if(!(cursor.x() < margen.left()) && !(cursor.x() > (this->window()->width()-margen.right())) &&
       !(cursor.y() < margen.top() ) && !(cursor.y() > (this->window()->height()-margen.bottom()))){
        int c = ((cursor.x()))  / celdaSz.width();
        int f = ((cursor.y()))  / celdaSz.height();
        cout<<"c: "<<c<<endl;
        if(f>-1 && f<f_ && c>-1 && c<c_){                                         //prevenir errores de calculo de pocos pixeles
            cout<<"Corresponde a la celda "<<f<<","<<c<<endl;
            ((celda*)layMapa_->itemAtPosition(f,c)->widget())->cambiarTipo(pintar_);
        }else{
            cout<<"Clic fuera del area"<<endl;
        }
    }else{
        cout<<"Clic fuera del area"<<endl;
    }
}

int mapa::getColumnas(){
    return c_;
}

int mapa::getFilas(){
    return f_;
}
