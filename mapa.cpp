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
#include <QDebug>
#include <unistd.h>
#include <thread>
#include <QTimer>
#include <chrono>
#include <QSlider>


using namespace std;

class MainWindow;

void mapa::zoom(int i){
    double restauracion = double(1/double(ultimoZoom_));
    view_->scale(restauracion,restauracion);
    if(i>ultimoZoom_){
        view_->scale(double(double(i)),double(double(i)));
    }else{
        view_->scale(double(double(i)),double(double(i)));
    }
    ultimoZoom_ = i;
}

void mapa::rastro(){
    if(robot_!=NULL){
        robot_->moveBy(1,0);
    }else{
        robot_ = pintarPixmap(1,2,new QPixmap("../I.A./recursos/robot.png"));
    }
}

mapa::mapa(int filas, int columnas, QProgressBar* barra, short a, short b,short c,short d,
           QWidget* parent) : QWidget(parent)
    {
    operacionesConstruccion(filas,columnas,barra);
    srand(time(NULL));
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            int aleatoriedad = rand()%100+ 1;
            if(i==0 || j==0 || i==f_-1 || j==c_-1){
                sustituirCelda(i,j,6);
            }else if(aleatoriedad<=a){
                sustituirCelda(i,j,5);
            }else if(aleatoriedad<=a+b){
                sustituirCelda(i,j,3);
            }else if(aleatoriedad<=a+b+c){
                sustituirCelda(i,j,1);
            }else if(aleatoriedad<=a+b+c+d){
                sustituirCelda(i,j,2);
            }else{
                sustituirCelda(i,j,4);
            }
            emit actualizarBarra(j+(i*c_));
        }
    }
    barra_->hide();
    layMapa_->addWidget(zoomSlider_);
    /*QTimer* contador = new QTimer(this);
    connect(contador,SIGNAL(timeout()),this,SLOT(rastro()));
    contador->start(10);*/
}

mapa::mapa(ifstream* fich, QProgressBar* barra, QWidget* parent) : QWidget(parent){
    *fich>>f_;
    *fich>>c_;
    operacionesConstruccion(f_,c_,barra);
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            short pixId;
            *fich>>pixId;
            sustituirCelda(i,j,pixId);
        }
    }
    barra_->hide();
    layMapa_->addWidget(zoomSlider_);
}

void mapa::operacionesConstruccion(int filas ,int columnas, QProgressBar* barra){
    f_=filas;
    c_=columnas;
    barra_=barra;
    dialogoAbrir_ = new QFileDialog(this);
    layMapa_ = new QBoxLayout(QBoxLayout::TopToBottom,this);
    matrizMapa_ = new celda[c_*f_];
    escena_ = new graphicsMapa(this);
    view_ = new QGraphicsView(escena_,this);
    terrenos_ = new QPixmap[7];
    terrenos_[6] = QPixmap("../I.A./recursos/muro.png");
    terrenos_[5] = QPixmap("../I.A./recursos/rojo.png");
    terrenos_[4] = QPixmap("../I.A./recursos/suelo.png");
    terrenos_[3] = QPixmap("../I.A./recursos/metal.png");
    terrenos_[2] = QPixmap("../I.A./recursos/tierra.png");
    terrenos_[1] = QPixmap("../I.A./recursos/rejilla.png");
    terrenos_[0] = QPixmap("../I.A./recursos/nuclear.png");
    zoomSlider_ = new QSlider(Qt::Horizontal,this);
    zoomSlider_->setMaximum(100);
    zoomSlider_->setMinimum(1);
    zoomSlider_->setValue(1);
    connect(zoomSlider_,SIGNAL(valueChanged(int)),this,SLOT(zoom(int)));
    ultimoZoom_ = 1;
    robot_ = NULL;
    if(f_>c_){
        escala_ = double(double((600)/f_)/terrenos_[0].size().height());
        view_->setMinimumSize((double(escala_*terrenos_[0].size().width())*c_)+2,600);
        view_->setBaseSize((double(escala_*terrenos_[0].size().width())*c_)+2,600);
        view_->setMaximumSize((double(escala_*terrenos_[0].size().width())*c_)+2,600);
    }else if(c_>f_){
        escala_ = double(double((600)/c_)/terrenos_[0].size().width());
        view_->setMinimumSize(600,(double(escala_*terrenos_[0].size().height())*f_)+2);
        view_->setBaseSize(600,(double(escala_*terrenos_[0].size().height())*f_)+2);
        view_->setMaximumSize(600,(double(escala_*terrenos_[0].size().height())*f_)+2);
    }else{
        escala_ = double(double((600)/c_)/terrenos_[0].size().width());
        view_->setMinimumSize(602,602);
        view_->setBaseSize(602,602);
        view_->setMaximumSize(602,602);
    }
    barra_->setMaximum(c_*f_);
    barra_->show();
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    layMapa_->addWidget(view_);
    view_->show();
}

QGraphicsPixmapItem* mapa::pintarPixmap(double fila, double columna, QPixmap* pix){
    if(fila >= 0 && fila<f_ && columna >= 0 && columna < c_){
        QGraphicsPixmapItem* auxPix;
        auxPix = escena_->addPixmap(*pix);
        auxPix->setScale(escala_);
        auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
        return auxPix;
    }else{
        return NULL;
    }
}

void mapa::sustituirCelda(double fila, double columna, short idPix){
    if(fila >= 0 && fila<f_ && columna >= 0 && columna < c_){
        QPixmap* pix = &terrenos_[idPix];
        QGraphicsPixmapItem* auxPixBorrar = matrizMapa_[pos(fila,columna)].pix_;
        QGraphicsPixmapItem* auxPix;
        auxPix = escena_->addPixmap(*pix);
        auxPix->setScale(escala_);
        auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
        if(auxPixBorrar==NULL){
            delete auxPixBorrar;
        }
        matrizMapa_[pos(fila,columna)].pix_=auxPix;
        matrizMapa_[pos(columna,fila)].tipo_=idPix;
    }
}

void mapa::pintar(){
    QPoint mousePos_ = this->mapFromGlobal(QCursor::pos());
    int anchoMapa  = view_->width();
    int altoMapa   = view_->height();
    int ratonX = (mousePos_.x());
    int ratonY = (mousePos_.y());
    if((ratonX > 0) && (mousePos_.x() < anchoMapa) && (ratonY > 0)  && (mousePos_.y() < altoMapa)){
        double  xCelda = anchoMapa / c_;
        double yCelda = altoMapa   / f_;
        double c = ratonX / xCelda;
        double f = ratonY / yCelda;
        int fila    = (int)(f);
        int columna = (int)(c);
        if(fila>-1 && fila<f_ && columna>-1 && columna<c_){
            sustituirCelda(fila,columna,5);
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

void mapa::movioMouse(QPointF mousePos){
    mousePos_ = mousePos;
    pintar();
}


