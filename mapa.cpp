#include "mapa.h"
#include "mainwindow.h"
#include "graphicsmapa.h"

#include <iostream>
#include <fstream>
#include <QScrollBar>
#include <QDebug>


using namespace std;

class MainWindow;


mapa::mapa(int filas, int columnas, QProgressBar* barra, short a, short b,short c,short d, QBoxLayout* lay,
           QWidget* parent) : QGraphicsView(parent)
    {
    operacionesConstruccion(filas,columnas,barra);
    layScrollAgentes_ = lay;
    parent_ = parent;
    srand(time(NULL));
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            int aleatoriedad = rand()%100+ 1;
            if(i==0 || j==0 || i==f_-1 || j==c_-1){
                sustituirCelda(i,j,muro);
            }else if(aleatoriedad<=a){
                sustituirCelda(i,j,rojo);
            }else if(aleatoriedad<=a+b){
                sustituirCelda(i,j,metal);
            }else if(aleatoriedad<=a+b+c){
                sustituirCelda(i,j,rejilla);
            }else if(aleatoriedad<=a+b+c+d){
                sustituirCelda(i,j,tierra);
            }else{
                sustituirCelda(i,j,suelo);
            }
            matrizMapa_[pos(i,j)].agente_ = NULL;
            emit actualizarBarra(j+(i*c_));
        }
    }
    barra_->hide();
}

mapa::mapa(ifstream* fich, QProgressBar* barra, QBoxLayout* lay, QWidget* parent) : QGraphicsView(parent){
    layScrollAgentes_ = lay;
    parent_ = parent;
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
}

void mapa::operacionesConstruccion(int filas ,int columnas, QProgressBar* barra){
    f_=filas;
    c_=columnas;
    barra_=barra;
    dialogoAbrir_ = new QFileDialog(this);
    matrizMapa_ = new celda[c_*f_];
    escena_ = new graphicsMapa(this);
    rastroTodos_ = true;
    this->setScene(escena_);
    graficosTerrenos_ = new QPixmap[7];
    graficosTerrenos_[muro]     = QPixmap("../I.A./recursos/muro.png");
    graficosTerrenos_[rojo]     = QPixmap("../I.A./recursos/rojo.png");
    graficosTerrenos_[suelo]    = QPixmap("../I.A./recursos/suelo.png");
    graficosTerrenos_[metal]    = QPixmap("../I.A./recursos/metal.png");
    graficosTerrenos_[tierra]   = QPixmap("../I.A./recursos/tierra.png");
    graficosTerrenos_[rejilla]  = QPixmap("../I.A./recursos/rejilla.png");
    graficosTerrenos_[nuclear]  = QPixmap("../I.A./recursos/nuclear.png");
    graficosAgente_ = new QPixmap[4];
    graficosAgente_[arriba]     = QPixmap("../I.A./recursos/robotArriba.png");
    graficosAgente_[abajo]      = QPixmap("../I.A./recursos/robotAbajo.png");
    graficosAgente_[derecha]    = QPixmap("../I.A./recursos/robotDerecha.png");
    graficosAgente_[izquierda]  = QPixmap("../I.A./recursos/robotIzquierda.png");
    simulando_ = false;
    ultimoZoom_ = 1;
    if(f_>c_){
        escala_ = (double(600)/f_)/double(graficosTerrenos_[0].size().height());
        this->setMinimumSize((double(escala_*graficosTerrenos_[0].size().width())*c_)+2,600);
        this->setBaseSize((double(escala_*graficosTerrenos_[0].size().width())*c_)+2,600);
        this->setMaximumSize((double(escala_*graficosTerrenos_[0].size().width())*c_)+2,600);
    }else if(c_>f_){
        escala_ = (double(600)/c_)/double(graficosTerrenos_[0].size().width());
        this->setMinimumSize(600,(double(escala_*graficosTerrenos_[0].size().height())*f_)+2);
        this->setBaseSize(600,(double(escala_*graficosTerrenos_[0].size().height())*f_)+2);
        this->setMaximumSize(600,(double(escala_*graficosTerrenos_[0].size().height())*f_)+2);
    }else{
        escala_ = (double(600)/f_)/double(graficosTerrenos_[0].size().width());
        this->setMinimumSize(602,602);
        this->setBaseSize(602,602);
        this->setMaximumSize(602,602);
    }
    emit actualizarBarra(0);
    barra_->setMaximum(c_*f_);
    barra_->show();
    connect(this,SIGNAL(actualizarBarra(int)),barra_,SLOT(setValue(int)));
    pincel_ = 5;
    tiempo_ = new QTimer(this);
    connect(tiempo_,SIGNAL(timeout()),this,SLOT(movimientoTempo()));;
    tiempo_->start(30);
}

void mapa::zoom(int i){
    this->scale(i*1/ultimoZoom_,i*1/ultimoZoom_);
    ultimoZoom_ = i;
}

QGraphicsPixmapItem* mapa::pintarPixmap(double fila, double columna, QPixmap* pix){
    if(fila >= 0 && fila<f_ && columna >= 0 && columna < c_){
        QGraphicsPixmapItem* auxPix;
        auxPix = escena_->addPixmap(*pix);
        auxPix->setScale(escala_);
        auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
        auxPix->setZValue(2);
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
    double anchoMapa  = this->width();
    double altoMapa   = this->height();
    double ratonX = (mousePos_.x());
    double ratonY = (mousePos_.y());
    if((ratonX > 0) && (mousePos_.x() < anchoMapa) && (ratonY > 0) && (mousePos_.y() < altoMapa)){
        double xCelda = anchoMapa  / c_;
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

QPoint mapa::getFilaColumna(QPointF P){
    int columna = int(int(P.x())/(32*escala_));
    int fila    = int(int(P.y())/(32*escala_));
    return QPoint(columna,fila);
}

void mapa::movioMouse(QPointF mousePos){
    mousePos_ = mousePos;
    pintar();
}


void mapa::cambiarTipoPincel(short tipo){
    pincel_ = tipo;
}

void mapa::movimientoTempo(){
    float valor;
    if(32*escala_>2){
        valor=1;
    }else if(32*escala_<2 && 32*escala_>0.8){
        valor=0.1;
    }else{
        valor=0.01;
    }
    for(int i=0;i<movimientosActuales_.size();i++){
        movimiento* mov = movimientosActuales_.at(i);
        if(mov->movRestante_>0){
            int id = mov->id_;
            if(mov->seguir_){
                float szHorizontal = this->horizontalScrollBar()->width();
                float szVertical = this->verticalScrollBar()->height();
                szHorizontal = (szHorizontal*mov->pix_->x())/this->width()*ultimoZoom_;
                szVertical = (szVertical*mov->pix_->y())/this->height()*ultimoZoom_;
                this->horizontalScrollBar()->setValue(szHorizontal);
                this->verticalScrollBar()->setValue(szVertical);
            }
            switch (mov->dir_){
            case arriba:
                mov->pix_->moveBy(0,-valor);
                break;
            case abajo:
                mov->pix_->moveBy(0,valor);
                break;
            case derecha:
                mov->pix_->moveBy(valor,0);
                break;
            default:
                mov->pix_->moveBy(-valor,0);
                break;
            }
            mov->movRestante_=mov->movRestante_-valor;
        }else{
            movimientosActuales_.removeAt(i);
            matrizMapa_[mapa::pos(mov->agente_->getY(),mov->agente_->getX())].agente_ = mov->agente_;
            mov->agente_->finMovimiento();
        }
    }
}

void mapa::agentePideMovimiento(agente* A, int id, int dir, QGraphicsPixmapItem* gPix, bool seguir, bool rastro){
    movimiento* aux = new movimiento;
    aux->dir_= dir;
    aux->pix_= gPix;
    aux->id_ = id;
    aux->movRestante_ = 32*escala_;
    aux->agente_ = A;
    aux->seguir_ = seguir;
    aux->pix_->setPixmap(graficosAgente_[dir]);
    movimientosActuales_.push_back(aux);
    matrizMapa_[mapa::pos(A->getY(),A->getX())].agente_ = NULL;
    if(rastro){
        QPixmap* pix = new QPixmap(graficosTerrenos_[0]);
        pix->fill(A->getColor());
        QGraphicsPixmapItem* aux = pintarPixmap(A->getY(),A->getX(),pix);
        aux->setZValue(1);
        aux->setOpacity(0.2);
    }
}

dirYPesos mapa::escanearEntorno(int x, int y){
    dirYPesos S;
    S.direccion_[arriba]    = (!(y<=1)   )*matrizMapa_[pos(y-1,x)].tipo_ +(y<=1)   *(-1);
    S.direccion_[abajo]     = (!(y>=f_-2))*matrizMapa_[pos(y+1,x)].tipo_ +(y>=f_-2)*(-1);
    S.direccion_[derecha]   = (!(x>=c_-2))*matrizMapa_[pos(y,x+1)].tipo_ +(x>=c_-2)*(-1);
    S.direccion_[izquierda] = (!(x<=1)   )*matrizMapa_[pos(y,x-1)].tipo_ +(x<=1)   *(-1);
    return S;
}

void mapa::addAgente(QPointF posReal){
    QPoint P = getFilaColumna(posReal);
    QGraphicsPixmapItem* gPix = (pintarPixmap(P.y(),P.x(),&graficosAgente_[1]));
    agente* aux = new agente("Agente"+QString::fromStdString(std::to_string(agentes_.size())),P.x(),P.y(),escala_*32,agentes_.size(),gPix,this);
    layScrollAgentes_->addWidget(aux);
    matrizMapa_[pos(P.y(),P.x())].agente_ = aux;
    agentes_.push_back(aux);
    aux->detontante();
    if(simulando_){
        aux->start();
    }
}

void mapa::startSimulacion(){
    if(!simulando_ && movimientosActuales_.size()==0 && agentes_.size()>0){
        for(int i=0;i<agentes_.size();i++){
            agentes_.at(i)->start();
        }
        simulando_=true;
    }else{
        for(int i=0;i<agentes_.size();i++){
            agentes_.at(i)->pause();
        }
        simulando_=false;
    }
}

void mapa::actualizarRastro(){
    int i=0;
    while(i<agentes_.size()){
        agentes_.at(i)->setRastro(rastroTodos_);
        i++;
    }
    rastroTodos_ = !rastroTodos_;
}

void mapa::actualizarSeguir(int id){
    int i=0;
    while(i<agentes_.size()){
        if(i!=id){
            agentes_.at(i)->unselectSeguir();
        }
        i++;
    }
}

void mapa::velocidad(int i){
    tiempo_->start(i);
}
