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
#include <QFileDialog>
#include <fstream>
#include "mainwindow.h"
#include <QDebug>

using namespace std;

struct vectors{
    int x;
    int y;
};

vectors camino[4];

class MainWindow;

mapa::mapa(ifstream* fich, QProgressBar* barra, QWidget* parent) : QWidget(parent){
    *fich>>f_;
    *fich>>c_;
    barra_=barra;
    layMapa_=new QGridLayout;
    layMapa_->setMargin(0);
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

    pixRobot_ = new QPixmap("../I.A./recursos/robot.gif");

    barra_->setMaximum(c_*f_);
    barra_->show();
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    emit actualizarBarra(0);
    int barI=1;
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            bool muro;
            *fich>>muro;
            layMapa_->addWidget(new celda(i,j,pixSuelo_,pixMuro_,muro,this),i,j);
            emit actualizarBarra(barI);
            barI++;
        }
    }
    setLayout(layMapa_);
    barra_->hide();
}


mapa::mapa(int filas, int columnas, QProgressBar* barra, int factor, QWidget* parent) : QWidget(parent){
    f_=filas;
    c_=columnas;
    srand(time(NULL));
    int aleatoriedad;
    barra_=barra;
    factor_=factor;
    dialogoAbrir_ = new QFileDialog(this);
    layMapa_ = new QGridLayout();
    layMapa_->setMargin(0);
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


    pixRobot_ = new QPixmap("../I.A./recursos/robot32x32.gif");
    barra_->show();
    barra_->setMaximum(c_*f_);
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
    setLayout(layMapa_);
    barra_->hide();
    qDebug() << "HolA";
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(gestorRobot()));
    timer_->start(1000);
    qDebug() << "Hola";

    ira_ = jra_ = 1;


    camino[0].x = 1;
    camino[0].y = 1;

    camino[1].x = 3;
    camino[1].y = 1;

    camino[2].x = 4;
    camino[2].y = 4;

    camino[3].x = 1;
    camino[3].y = 4;

    camino_ = 0;
}

void mapa::cambiarCeldaEn(int i, int j, bool muro){
    ((celda*) (layMapa_->itemAtPosition(i,j)->widget()))->cambiarTipo(muro);
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
                ((celda*) (layMapa_->itemAtPosition(i,j)->widget()))->cambiarTipo(true);
            }else{
                ((celda*) (layMapa_->itemAtPosition(i,j)->widget()))->cambiarTipo(false);
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
                ((celda*)layMapa_->itemAtPosition(i,j)->widget())->cambiarTipo(true);
            }else{
                ((celda*)layMapa_->itemAtPosition(i,j)->widget())->cambiarTipo(false);
            }
            barI++;
            emit actualizarBarra(barI);
        }
    }
    barra_->hide();

    celdaSize_ = layMapa_->itemAtPosition(1,1)->widget()->size();
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
            *fich<<!(((celda*)layMapa_->itemAtPosition(i,j)->widget())->atravesable())<<" ";
        }
        *fich<<endl;
    }
    ((MainWindow*)this->parent())->actualizarTitulo(false);
}


//ROBOT MANAGEMENT
void mapa::gestorRobot()
{
    if(ira_ >= getFilas()-1){ ira_ = 1;jra_=1;}
    moverRobot(camino[camino_ % 4].x,camino[camino_++ % 4].y);

}

void mapa::moverRobot(int i, int j){
    quitarRobotDeActual();

    qDebug() << "Camino actual : " << camino[camino_].x << " , " << camino[camino_].y ;
    ponerRobotEn(i,j);
    ira_ = i;
    jra_ = j;
}


void mapa::quitarRobotDeActual(){ //[i,j]ra_ = i,j actual position of robot
    bool atravesable = ((celda*) (layMapa_->itemAtPosition(ira_,jra_)->widget()))->atravesable();
    delete ((robot*) (layMapa_->itemAtPosition(ira_,jra_)->widget())); //limpiamos
    layMapa_->addWidget (new celda(ira_,jra_,pixSuelo_,pixMuro_,!atravesable,this),ira_,jra_); //colocamos nuevo

}

void mapa::ponerRobotEn(int i, int j){
    layMapa_->addWidget (new robot(i,j,pixRobot_,this),i,j);
}

void mapa::reiniciarRobot(){
    moverRobot(1,1);
}


