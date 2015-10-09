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
#include <math.h>
#include <QPalette>
#include <QRect>

using namespace std;

mapa::mapa(int filas, int columnas, QProgressBar* barra_,int factor, QWidget* parent) : QWidget(parent){
    f_=filas;
    c_=columnas;
    srand(time(NULL));
    int aleatoriedad;
    layMapa_ = new QGridLayout();
    layMapa_->setMargin(0);
    if(filas<35 && columnas<35){
        pixSuelo_ = new QPixmap("../I.A./recursos/suelo.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muro.png");
        layMapa_->setSpacing(0);
    }else{
        pixSuelo_ = new QPixmap("../I.A./recursos/sueloLow.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muroLow.png");
        layMapa_->setSpacing(1);
        QPalette fondo(palette());
        fondo.setColor(QPalette::Background, Qt::black);
        setAutoFillBackground(true);
        setPalette(fondo);
    }
    barra_->show();
    barra_->setMaximum(columnas*filas);
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    int barI=1;
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            aleatoriedad = rand()%100+ 1;
            if(aleatoriedad<=factor || (i == 0 || j == 0 || i == f_-1 || j == c_-1)){
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

void mapa::cambiarCeldaEn(int i, int j, bool muro){
    ((celda*) (layMapa_->itemAtPosition(i,j)->widget()))->cambiarTipo(muro);
}


void mapa::limpiarMapa(){
    cout << "Entramos en limpiarMApa" << endl;
    for(int i = 0; i < f_ ; i++)
        for(int j = 0; j < c_ ; j++)
            ((celda*) (layMapa_->itemAtPosition(i,j)->widget()))->cambiarTipo(false);
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
    QRect widgetRect = this->geometry();

    int anchoMapa  = widgetRect.width();
    int altoMapa   = widgetRect.height();
    int xFinalMapa = widgetRect.width();
    int yFinalMapa = widgetRect.height();

    int ratonX = (cursor.x());
    int ratonY = (cursor.y());
        if((ratonX > 0) && (cursor.x() < xFinalMapa) &&
           (ratonY > 0)  && (cursor.y() < yFinalMapa)){
            double  xCelda = anchoMapa / c_;
            double yCelda = altoMapa   / f_;

            cout<<"Las celdas miden "<<xCelda<<","<<yCelda<<endl;

            double c = ratonX / xCelda;
            double f = ratonY / yCelda;

            int fila    = (int)(f);
            int columna = (int)(c);

            cout<<ratonX<<" "<<ratonY<<endl;

            if(fila>-1 && fila<f_ && columna>-1 && columna<c_){                                         //prevenir errores de calculo de pocos pixeles
                cout<<"Corresponde a la celda "<<fila<<","<<columna<<endl;
                ((celda*)layMapa_->itemAtPosition(fila,columna)->widget())->cambiarTipo(pintar_);
            }
    }else{
        cout<<"Fuera"<<endl;
    }
}

int mapa::getColumnas(){
    return c_;
}

int mapa::getFilas(){
    return f_;
}
