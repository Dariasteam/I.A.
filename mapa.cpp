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

mapa::mapa(int filas, int columnas, QProgressBar* barra, int factor, QWidget* parent) : QWidget(parent){
    operacionesConstruccion(filas,columnas,factor,barra);
    srand(time(NULL));
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            int aleatoriedad = rand()%100+ 1;
            if(i==0 || j==0 || i==f_-1 || j==c_-1){
                cargarCelda(6,i,j);
            }else if(aleatoriedad<=factor){
                cargarCelda(5,i,j);
            }else{
                aleatoriedad = rand()%4+ 1;
                cargarCelda(aleatoriedad,i,j);
            }
            emit actualizarBarra(j+(i*c_));
        }
    }
    barra_->hide();
}

mapa::mapa(ifstream* fich, QProgressBar* barra, QWidget* parent) : QWidget(parent){
    *fich>>f_;
    *fich>>c_;
    operacionesConstruccion(f_,c_,0,barra);
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            short muro;
            *fich>>muro;
            cargarCelda(muro,i,j);
        }
    }
    barra_->hide();
}

void mapa::operacionesConstruccion(int filas ,int columnas, int factor, QProgressBar* barra){
    f_=filas;
    c_=columnas;
    barra_=barra;
    factor_=factor;
    dialogoAbrir_ = new QFileDialog(this);
    layMapa_ = new QBoxLayout(QBoxLayout::TopToBottom,this);

    barra_->setMaximum(c_*f_);
    barra_->show();
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    matrizMapa_ = new celda[c_*f_];
    escena_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(escena_,this);
    if(f_<35 && c_<35){
        pixSuelo_ = new QPixmap("../I.A./recursos/suelo.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muro.png");
        pixRojo_ = new QPixmap("../I.A./recursos/rojo.png");
        pixMetal_ = new QPixmap("../I.A./recursos/metal.png");
        pixTierra_ = new QPixmap("../I.A./recursos/tierra.png");
        pixNuclear_ = new QPixmap("../I.A./recursos/nuclear.png");
        pixRejilla_ = new QPixmap("../I.A./recursos/rejilla.png");

    }else{
        /*pixSuelo_ = new QPixmap("../I.A./recursos/sueloLow.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muroLow.png");
        layMapa_->setSpacing(0);*/
        pixSuelo_ = new QPixmap("../I.A./recursos/suelo.png");
        pixMuro_ = new QPixmap("../I.A./recursos/muro.png");
        pixRojo_ = new QPixmap("../I.A./recursos/rojo.png");
        pixMetal_ = new QPixmap("../I.A./recursos/metal.png");
        pixTierra_ = new QPixmap("../I.A./recursos/tierra.png");
        pixNuclear_ = new QPixmap("../I.A./recursos/nuclear.png");
        pixRejilla_ = new QPixmap("../I.A./recursos/rejilla.png");
        layMapa_->setSpacing(0);
    }
    layMapa_->setSpacing(0);
    escala_ = ((this->width()*5/pixMuro_->size().width()));
    escala_ = escala_ / f_;
    view_->setBaseSize(500,500);
    view_->setMinimumSize(500,500);
    view_->setMaximumSize(500,500);

    layMapa_->setContentsMargins(0,0,0,0);

    layMapa_->addWidget(view_);
    view_->show();

    view_->setSizeIncrement(1,0.5);
}

QPixmap* mapa::cargarCelda(short npix, int i, int j){
    switch (npix) {
    case 1:
        matrizMapa_[pos(i,j)]={npix,pintarPixmap(i,j,pixSuelo_)};
        break;
    case 2:
        matrizMapa_[pos(i,j)]={npix,pintarPixmap(i,j,pixTierra_)};
        break;
    case 3:
        matrizMapa_[pos(i,j)]={npix,pintarPixmap(i,j,pixMetal_)};
        break;
    case 4:
        matrizMapa_[pos(i,j)]={npix,pintarPixmap(i,j,pixRejilla_)};
        break;
    case 5:
        matrizMapa_[pos(i,j)]={npix,pintarPixmap(i,j,pixRojo_)};
        break;
    case 6:
        matrizMapa_[pos(i,j)]={npix,pintarPixmap(i,j,pixMuro_)};
        break;
    }
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
    cout<<"skdjfsldjflskddjfasf"<<endl;
    pintar();

}

QGraphicsPixmapItem* mapa::pintarPixmap(double x, double y, QPixmap* pix){
    QGraphicsPixmapItem* auxPix;
    auxPix = escena_->addPixmap(*pix);
    auxPix->setScale(escala_);
    auxPix->setPos(x*escala_*pix->size().height(),y*escala_*pix->size().height());
    return auxPix;
}

void mapa::sustituirPixmap(double x, double y, QPixmap* pix){
    QGraphicsPixmapItem* auxPixBorrar = matrizMapa_[pos(x,y)].pix_;
    QGraphicsPixmapItem* auxPix;
    auxPix = escena_->addPixmap(*pix);
    auxPix->setScale(escala_);
    auxPix->setPos(x*escala_*pix->size().height(),y*escala_*pix->size().height());
    matrizMapa_[pos(x,y)].pix_=auxPix;
    delete auxPixBorrar;
    matrizMapa_[pos(x,y)].tipo_=6;
}


void mapa::pintar(){
    QPoint cursor = this->mapFromGlobal(QCursor::pos());
    int anchoMapa  = view_->width();
    int altoMapa   = view_->height();


    int ratonX = (cursor.x());
    int ratonY = (cursor.y());

    if((ratonX > 0) && (cursor.x() < anchoMapa) &&
       (ratonY > 0)  && (cursor.y() < altoMapa)){
        double  xCelda = anchoMapa / c_;
        double yCelda = altoMapa   / f_;
        double c = ratonX / xCelda;
        double f = ratonY / yCelda;

        int fila    = (int)(f);
        int columna = (int)(c);

        if(fila>-1 && fila<f_ && columna>-1 && columna<c_){
            //cargarCelda(pintar_,fila,columna);
            sustituirPixmap(columna,fila,pixMuro_);
        }
    }
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
            *fich<<matrizMapa_[pos(i,j)].tipo_<<" ";
        }
        *fich<<endl;
    }
    ((MainWindow*)this->parent())->actualizarTitulo(false);
}

int mapa::pos(int f,int c){
    int aux = f*c_;
    return aux + c;
}


