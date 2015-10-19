#include "mapa.h"
#include "mainwindow.h"
#include "graphicsmapa.h"

#include <iostream>
#include <math.h>
#include <fstream>



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
    for(int i=0;i<1;i++){
        agente* aux = new agente(f_/2,c_/2,i,pintarPixmap(f_/2,c_/2,&graficosAgente_[0]),this);
        aux->movimiento_ = escala_*32;
        agentes_.push_back(aux);
    }
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
    graficosTerrenos_ = new QPixmap[7];
    graficosTerrenos_[6] = QPixmap("../I.A./recursos/muro.png");
    graficosTerrenos_[5] = QPixmap("../I.A./recursos/rojo.png");
    graficosTerrenos_[4] = QPixmap("../I.A./recursos/suelo.png");
    graficosTerrenos_[3] = QPixmap("../I.A./recursos/metal.png");
    graficosTerrenos_[2] = QPixmap("../I.A./recursos/tierra.png");
    graficosTerrenos_[1] = QPixmap("../I.A./recursos/rejilla.png");
    graficosTerrenos_[0] = QPixmap("../I.A./recursos/nuclear.png");
    graficosAgente_ = new QPixmap[4];
    graficosAgente_[0] = QPixmap("../I.A./recursos/robotArriba.png");
    graficosAgente_[1] = QPixmap("../I.A./recursos/robotAbajo.png");
    graficosAgente_[2] = QPixmap("../I.A./recursos/robotDerecha.png");
    graficosAgente_[3] = QPixmap("../I.A./recursos/robotIzquierda.png");
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
        escala_ = (double(600)/f_)/double(graficosTerrenos_[0].size().height());
        view_->setMinimumSize((double(escala_*graficosTerrenos_[0].size().width())*c_)+2,600);
        view_->setBaseSize((double(escala_*graficosTerrenos_[0].size().width())*c_)+2,600);
        view_->setMaximumSize((double(escala_*graficosTerrenos_[0].size().width())*c_)+2,600);
    }else if(c_>f_){
        escala_ = (double(600)/c_)/double(graficosTerrenos_[0].size().width());
        view_->setMinimumSize(600,(double(escala_*graficosTerrenos_[0].size().height())*f_)+2);
        view_->setBaseSize(600,(double(escala_*graficosTerrenos_[0].size().height())*f_)+2);
        view_->setMaximumSize(600,(double(escala_*graficosTerrenos_[0].size().height())*f_)+2);
    }else{
        escala_ = (double(600)/f_)/double(graficosTerrenos_[0].size().width());
        view_->setMinimumSize(602,602);
        view_->setBaseSize(602,602);
        view_->setMaximumSize(602,602);
    }
    emit actualizarBarra(0);
    barra_->setMaximum(c_*f_);
    barra_->show();
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    tiempo_ = new QTimer(this);
    connect(tiempo_,SIGNAL(timeout()),this,SLOT(movimientoTempo()));
    tiempo_->start(15);
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
                    switch (agentes_.at(i)->dir_){ //1 Arriba, 2 Abajo, 3 Derecha, 4 Izquierda
                    case 1:
                        agentes_.at(i)->getPix()->moveBy(0,-1);
                        break;
                    case 2:
                        agentes_.at(i)->getPix()->moveBy(0,1);
                        break;
                    case 3:
                        agentes_.at(i)->getPix()->moveBy(1,0);
                        break;
                    default:
                        agentes_.at(i)->getPix()->moveBy(-1,0);
                        break;
                    }
                    agentes_.at(i)->tiempoMov_--;
                }else{
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
        QPixmap* pix = &graficosTerrenos_[idPix];
        QGraphicsPixmapItem* auxPixBorrar = matrizMapa_[pos(fila,columna)].pix_;
        QGraphicsPixmapItem* auxPix;
        auxPix = escena_->addPixmap(*pix);
        auxPix->setScale(escala_);
        auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
        auxPix->setZValue(-1);
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
    if(f<f_ && f>=0 && c<c_ && c>=0){
        return (f*c_) + c;
    }else{
        return 0;
    }
}

void mapa::movioMouse(QPointF mousePos){
    mousePos_ = mousePos;
    pintar();
}


void mapa::cambiarTipoPincel(short tipo){
    pincel_ = tipo;
}

void mapa::agenteFin(int id){
    agentes_.at(id)->getPix()->setPixmap(graficosAgente_[(agentes_.at(id)->dir_)-1]);
    agentesActivos_++;
}

dirYPesos mapa::escanearEntorno(int x, int y){          //0 Arriba, 1 Abajo, 2 Derecha, 3 Izquierda
    dirYPesos S;
    S.direccion_[0] = (!(y<=1)   )*matrizMapa_[pos(y-1,x)].tipo_ +(y<=1)   *(-1);
    S.direccion_[1] = (!(y>=f_-2))*matrizMapa_[pos(y+1,x)].tipo_ +(y>=f_-2)*(-1);
    S.direccion_[2] = (!(x>=c_-2))*matrizMapa_[pos(y,x+1)].tipo_ +(x>=c_-2)*(-1);
    S.direccion_[3] = (!(x<=1)   )*matrizMapa_[pos(y,x-1)].tipo_ +(x<=1)   *(-1);
    return S;
}


