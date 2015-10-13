#include "mapa.h"
#include <iostream>
#include <QMouseEvent>
#include <QResizeEvent>
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
#include <QFileDialog>
#include <fstream>
#include "mainwindow.h"
#include <QPainter>
#include <QRect>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QSizePolicy>
#include <QBoxLayout>

using namespace std;

class MainWindow;

void mapa::operacionesConstruccion(int filas ,int columnas, int factor, QProgressBar* barra){
    f_=filas;
    c_=columnas;
    barra_=barra;
    factor_=factor;
    dialogoAbrir_ = new QFileDialog(this);
    layMapa_ = new QBoxLayout(QBoxLayout::TopToBottom,this);
    barra_->show();
    barra_->setMaximum(c_*f_);
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    matrizMapa_ = new bool[c_*f_];
    escena_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(escena_,this);
    if(f_<35 && c_<35){
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
    escala_ = ((this->width()*5/pixMuro_->size().width()));
    escala_ = escala_ / f_;
}

mapa::mapa(ifstream* fich, QProgressBar* barra, QWidget* parent) : QWidget(parent){
    *fich>>f_;
    *fich>>c_;
    operacionesConstruccion(f_,c_,0,barra);
    int barI=1;
    QGraphicsPixmapItem* auxPix;
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            bool muro;
            *fich>>muro;
            matrizMapa_[pos(i,j)]=muro;
            if(muro){
                auxPix = escena_->addPixmap(*pixMuro_);
            }else{
                auxPix = escena_->addPixmap(*pixSuelo_);
            }
            emit actualizarBarra(barI);
            barI++;
            auxPix->setScale(escala_);
            auxPix->setPos(i*escala_*pixMuro_->size().height(),j*escala_*pixMuro_->size().height());
        }
    }
    setLayout(layMapa_);
    barra_->hide();
}


mapa::mapa(int filas, int columnas, QProgressBar* barra, int factor, QWidget* parent) : QWidget(parent){
    operacionesConstruccion(filas,columnas,factor,barra);
    srand(time(NULL));
    int aleatoriedad;
    int barI=1;
    QGraphicsPixmapItem* auxPix;
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            aleatoriedad = rand()%100+ 1;
            if(aleatoriedad<=factor || (i==0 || j==0 || i==f_-1 || j==c_-1)){
                auxPix = escena_->addPixmap(*pixMuro_);
                matrizMapa_[pos(i,j)]=true;
            }else{
                auxPix = escena_->addPixmap(*pixSuelo_);
                matrizMapa_[pos(i,j)]=false;
            }
            emit actualizarBarra(barI);
            barI++;
            auxPix->setScale(escala_);
            auxPix->setPos(i*escala_*pixMuro_->size().height(),j*escala_*pixMuro_->size().height());
        }
    }
    view_->show();
    layMapa_->addWidget(view_);
    barra_->hide();
}

void mapa::cambiarCeldaEn(int i, int j, bool muro){
    matrizMapa_[pos(i,j)] = muro;
}


void mapa::limpiarMapa(){
    cout << "Entramos en limpiarMApa" << endl;
    barra_->setMaximum(c_*f_);
    barra_->setValue(0);
    barra_->show();
    int barI = 1;
    for(int i = 0; i < f_ ; i++){
        for(int j = 0; j < c_ ; j++){
            if((i == 0 || j == 0 || i == f_-1 || j == c_-1)){
                matrizMapa_[pos(i,j)] = true;
            }else{
                matrizMapa_[pos(i,j)] = false;
            }
            barI++;
            emit actualizarBarra(barI);
        }
    }
    barra_->hide();
}

void mapa::actualizarEsteMapa(int factor){
    int aleatoriedad;
    barra_->show();
    emit actualizarBarra(0);
    int barI=1;

    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            aleatoriedad = rand()%100+ 1;
            if(aleatoriedad<=factor || (i == 0 || j == 0 || i == f_-1 || j == c_-1)){
                matrizMapa_[pos(i,j)] = true;
            }else{
                matrizMapa_[pos(i,j)] = false;
            }
            barI++;
            emit actualizarBarra(barI);
        }
    }
    barra_->hide();
}

void mapa::mousePressEvent(QMouseEvent* E){
    ((MainWindow*)this->parent())->actualizarTitulo(true);
    if(E->button()==Qt::LeftButton){
        pintar_=true;
    }else if(E->button()==Qt::RightButton){
        pintar_=false;
    }
    pintar();
}

void mapa::resizeEvent(QResizeEvent* ){
   layMapa_->update();

}


void mapa::mouseMoveEvent(QMouseEvent* ){
    pintar();

}

void mapa::pintar(){
    //paintEngine()->drawPixmap(10,10,10,10,*pixMuro_);


    //paintEvent();
    //lienzo_->



    /*QSize celdaSz = layMapa_->itemAtPosition(0,0)->widget()->size();
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



        /*if(fila>-1 && fila<f_ && columna>-1 && columna<c_){                                         //prevenir errores de calculo de pocos pixeles
            cout<<"Corresponde a la celda "<<fila<<","<<columna<<endl;
            //((celda*)layMapa_->itemAtPosition(fila,columna)->widget())->cambiarTipo(pintar_);
            matrizMapa_[pos(fila,columna)]=pintar_;
        }
    }*/
}

int mapa::getColumnas(){
    return c_;
}

int mapa::getFilas(){
    return f_;
}


void mapa::guardar(ofstream* fich){
    *fich<<f_<<" "<<c_<<endl;
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            *fich<<matrizMapa_[pos(i,j)]<<" ";
        }
        *fich<<endl;
    }
    ((MainWindow*)this->parent())->actualizarTitulo(false);
}

int mapa::pos(int f,int c){
    int aux = f*c_;
    return aux + c;
}


