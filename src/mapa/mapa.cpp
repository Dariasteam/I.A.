#include "mapa.h"

#include <iostream>
#include <cmath>

using namespace std;

mapa::mapa(int filas, int columnas, QProgressBar* barra, short a, short b,short c,short d,
           QPixmap* graficosTerrenos, QWidget* parent) : QGraphicsView(){
    f_ =filas;
    c_ =columnas;
    parent_ = parent;
    graficosTerrenos_=graficosTerrenos;
    operacionesConstruccion(barra);
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
            mapa_[pos(i,j)].agente_ = NULL;
            //emit actualizarBarra(j+(i*c_));
        }
    }
    barra_->hide();
}

mapa::mapa(std::ifstream* fich, QProgressBar *barra, QPixmap* graficosTerrenos, QWidget *parent) : QGraphicsView(){
    *fich>>f_;
    *fich>>c_;
    parent_ = parent;
    graficosTerrenos_=graficosTerrenos;
    operacionesConstruccion(barra);
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            short pixId;
            *fich>>pixId;
            sustituirCelda(i,j,pixId);
        }
    }
    barra_->hide();
}

mapa::mapa(int f, int c, QPixmap* graficosTerrenos, QWidget *parent){
    f_=f;
    c_=c;
    parent_=parent;
    graficosTerrenos_=graficosTerrenos;
    operacionesConstruccion(NULL);
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            sustituirCelda(i,j,-10);
            mapa_[pos(i,j)].pix_=NULL;
        }
    }
}

void mapa::operacionesConstruccion(QProgressBar* b){
    barra_ = b;
    mapa_ = new celda[c_*f_];
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
    //connect(this,&mapa::actualizarBarra,barra_,&QProgressBar::setValue);
    //emit actualizarBarra(0);
    if(barra_!=NULL){
        barra_->setMaximum(c_*f_);
        barra_->show();
    }
    escena_ = new graphicsMapa(parent_);
    setScene(escena_);
}

void mapa::zoom(int i){
    this->scale(i*1/ultimoZoom_,i*1/ultimoZoom_);
    ultimoZoom_ = i;
}

celda* mapa::getCelda(int f, int c){
    if(f<f_ && f>-1 && c<c_ && c>-1 && mapa_[pos(f,c)].tipo_ > -1){
        return &mapa_[pos(f,c)];
    }else{
        return NULL;
    }
}

double mapa::getEscala(){
    return escala_;
}

int mapa::getColumnas(){
    return c_;
}

int mapa::getFilas(){
    return f_;
}

int mapa::pos(int f,int c){
    if(f<f_ && f>=0 && c<c_ && c>=0){
        return (f*c_) + c;
    }else{
        return 0;
    }
}

void mapa::setValorCelda(int x, int y, short v){
    mapa_[pos(y,x)].tipo_ = v;
}

QGraphicsPixmapItem* mapa::pintarPixmap(double fila, double columna, QPixmap* pix){
    if(fila >= 0 && fila<f_ && columna >= 0 && columna < c_){
        QGraphicsPixmapItem* auxPix;
        auxPix = escena_->addPixmap(*pix);
        auxPix->setScale(escala_);
        auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
        auxPix->setZValue(3);
        return auxPix;
    }else
        return NULL;
 }

void mapa::sustituirCelda(double fila, double columna, short idPix){
    if(mapa_[pos(fila,columna)].tipo_ != idPix || mapa_[pos(fila,columna)].pix_==NULL){
        if(fila >= 0 && fila<f_ && columna >= 0 && columna < c_){
            QPixmap* pix = &graficosTerrenos_[idPix];
            QGraphicsPixmapItem* auxPixBorrar = mapa_[pos(fila,columna)].pix_;
            QGraphicsPixmapItem* auxPix;
            auxPix = escena_->addPixmap(*pix);
            auxPix->setScale(escala_);
            auxPix->setPos(columna*escala_*pix->size().height(),fila*escala_*pix->size().height());
            auxPix->setZValue(-1);
            if(auxPixBorrar==NULL){
                delete auxPixBorrar;
            }
            mapa_[pos(fila,columna)].pix_=auxPix;
            mapa_[pos(fila,columna)].x_=columna;
            mapa_[pos(fila,columna)].y_=fila;
            mapa_[pos(fila,columna)].tipo_=idPix;
            if(idPix == nuclear){
               objetivos_.push_back(new objetivo{&mapa_[pos(fila,columna)],false});
            }
        }
    }
}

QPoint mapa::getFilaColumna(QPointF P){
    int columna = int(int(P.x())/(32*escala_));
    int fila    = int(int(P.y())/(32*escala_));
    return QPoint(columna,fila);
}

short* mapa::escanearEntorno(int x, int y){
    short* direccion_ = new short[4];
    direccion_[arriba]    =  (!(y<=1)   )*mapa_[pos(y-1,x)].tipo_ +(y<=1)   *(-1);
    direccion_[abajo]     =  (!(y>=f_-2))*mapa_[pos(y+1,x)].tipo_ +(y>=f_-2)*(-1);
    direccion_[derecha]   =  (!(x>=c_-2))*mapa_[pos(y,x+1)].tipo_ +(x>=c_-2)*(-1);
    direccion_[izquierda] =  (!(x<=1)   )*mapa_[pos(y,x-1)].tipo_ +(x<=1)   *(-1);
    return direccion_;
}

void mapa::setCelda(int x, int y, short v){
    if(v==-1){
        v=6;
    }
    mapa_[pos(y,x)].tipo_ = v;
    sustituirCelda(x,y,v);
}

int mapa::getCoste(celda *pos, celda *objetivo){
    return abs(pos->x_ - objetivo->x_) + abs(pos->y_ - objetivo->y_);               //MANHATTAN
    //return sqrt(abs(pos->x_ - objetivo->x_)^2 + abs(pos->y_ - objetivo->y_)^2);   //EUCLIDES
}

void celda::imprimir(){
    cout<<"("<<y_<<","<<x_<<")["<<tipo_<<"]";
}


