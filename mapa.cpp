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
#include <QDebug>
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
#include <QThread>
#include <QTimer>
#include <chrono>
#include <QSlider>
#include "graphicsmapa.h"


using namespace std;

class MainWindow;


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
    agentes_.push_back(new agente(5,5,0,pintarPixmap(5,5,new QPixmap("../I.A./recursos/robot.png")),this));
    agentes_.push_back(new agente(3,3,1,pintarPixmap(3,3,new QPixmap("../I.A./recursos/robot.png")),this));
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
    zoomSlider_->setRange(1,100);
    zoomSlider_->setValue(1);
    layMapa_->addWidget(view_);
    layMapa_->addWidget(zoomSlider_);
    idAgente_ = 0;
    error = 0;
    connect(zoomSlider_,SIGNAL(valueChanged(int)),this,SLOT(zoom(int)));
    ultimoZoom_ = 1;
    if(f_>c_){
        escala_ = (double(600)/f_)/double(terrenos_[0].size().height());
        view_->setMinimumSize((double(escala_*terrenos_[0].size().width())*c_)+2,600);
        view_->setBaseSize((double(escala_*terrenos_[0].size().width())*c_)+2,600);
        view_->setMaximumSize((double(escala_*terrenos_[0].size().width())*c_)+2,600);
    }else if(c_>f_){
        escala_ = (double(600)/c_)/double(terrenos_[0].size().width());
        view_->setMinimumSize(600,(double(escala_*terrenos_[0].size().height())*f_)+2);
        view_->setBaseSize(600,(double(escala_*terrenos_[0].size().height())*f_)+2);
        view_->setMaximumSize(600,(double(escala_*terrenos_[0].size().height())*f_)+2);
    }else{
        escala_ = (double(600)/f_)/double(terrenos_[0].size().width());
        view_->setMinimumSize(602,602);
        view_->setBaseSize(602,602);
        view_->setMaximumSize(602,602);
    }
    barra_->show();
    barra_->setMaximum(c_*f_);
    barra_->show();
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    tiempo_ = new QTimer(this);
    connect(tiempo_,SIGNAL(timeout()),this,SLOT(movimientoTempo()));
    tiempo_->start(5);
    agentesActivos_ = 0;
    pincel_ = 5;
}

void mapa::zoom(int i){
    view_->scale(i*1/ultimoZoom_,i*1/ultimoZoom_);
    ultimoZoom_ = i;
}

void mapa::movimientoTempo(){
    if(agentesActivos_ > 0){
        for(int i=0;i<agentes_.size();i++){
            if(agentes_.at(i)->finCalculo_==true){
                if(agentes_.at(i)->tiempoMov_>0){
                    switch (agentes_.at(i)->direccion_){ //0 Arriba, 1 Abajo, 2 Derecha, 3 Izquierda
                    case 0:
                        agentes_.at(i)->getPix()->moveBy(0,-1);
                        break;
                    case 1:
                        agentes_.at(i)->getPix()->moveBy(0,1);
                        break;
                    case 2:
                        agentes_.at(i)->getPix()->moveBy(1,0);
                        break;
                    default:
                        agentes_.at(i)->getPix()->moveBy(-1,0);
                        break;
                    }
                    agentes_.at(i)->tiempoMov_--;
                }else{
                    agentes_.at(i)->finCalculo_=false;
                    agentes_.at(i)->finMovimiento();
                    agentesActivos_--;
                }
            }
        }
    }
}


QGraphicsPixmapItem* mapa::pintarPixmap(double fila, double columna, QPixmap* pix){
    if(fila >= 0 && fila<f_ && columna >= 0 && columna < c_){
        QGraphicsPixmapItem* auxPix;
        auxPix = escena_->addPixmap(*pix);
        auxPix->setScale(escala_);
        auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
        return auxPix;
    }else
        return NULL;
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
        matrizMapa_[pos(fila,columna)].tipo_=idPix;
    }
}

void mapa::pintar(){
    double anchoMapa  = view_->width();
    double altoMapa   = view_->height();

    int ratonX = (mousePos_.x());
    int ratonY = (mousePos_.y());
    if((ratonX > 0) && (mousePos_.x() < anchoMapa) && (ratonY > 0)  && (mousePos_.y() < altoMapa)){
        double xCelda = anchoMapa / c_;
        double yCelda = altoMapa   / f_;
        double c = ratonX / xCelda;
        double f = ratonY / yCelda;
        int fila    = (int)(f);
        int columna = (int)(c);
        if(fila>-1 && fila<f_ && columna>-1 && columna<c_){
            sustituirCelda(fila,columna,pincel_);
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


void mapa::cambiarTipoPincel(short tipo){
    pincel_ = tipo;
}

void mapa::agenteFin(int id){
    cout<<"El agente "<<id<<" ha finalizado sus cálculos"<<endl;
    agentesActivos_++;
}


