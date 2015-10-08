#include "mapa.h"
#include <iostream>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QGridLayout>

using namespace std;

mapa::mapa(int filas, int columnas) : QWidget(){
    f_=filas;
    c_=columnas;
    layMapa_ = new QGridLayout;
    layMapa_->setSpacing(0);
    for(int i=0;i<f_;i++){
        for(int j=0;j<c_;j++){
            cout<<"Generando la celda "<<i<<","<<j<<endl;
            layMapa_->addWidget(new celda(i,j),i,j);
        }
    }
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
        int c = ((cursor.x())-margen.left()) /celdaSz.width();
        int f = ((cursor.y())-margen.top())  /celdaSz.height();
        if(f>-1 && f<f_ && c>-1 && c<c_){                                         //prevenir errores de calculo de pocos pixeles
            cout<<"Corresponde a la celda "<<c<<","<<f<<endl;
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
