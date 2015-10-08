#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QBoxLayout>
#include "celda.h"
#include <QMouseEvent>
#include <QPoint>
#include <iostream>
#include <QMargins>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setMouseTracking(true);
    ui->setupUi(this);
    widPrincipal_ = new QWidget();
    layPrincipal_ = new QGridLayout();
    f_=10;
    c_=10;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            celda* aux = new celda(i,j);
            layPrincipal_->addWidget(aux,i,j);
        }
    }
    widPrincipal_->setLayout(layPrincipal_);
    this->setCentralWidget(widPrincipal_);
    layPrincipal_->setSpacing(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Calcular si la pulsacion se produce en el interior de la ventana
void MainWindow::mouseMoveEvent(QMouseEvent* E){
    QSize celdaSz = layPrincipal_->itemAtPosition(0,0)->widget()->size();
    QPoint cursor = widPrincipal_->mapFromGlobal(QCursor::pos());
    cout<<"Cursor "<<cursor.x()<<","<<cursor.y()<<endl;
    QMargins margen = layPrincipal_->contentsMargins();
    if(!(cursor.x() < margen.left()) && !(cursor.x() > (this->window()->width()-margen.right())) &&
       !(cursor.y() < margen.top() ) && !(cursor.y() > (this->window()->height()-50))){
        int c = ((cursor.x())-margen.left())/celdaSz.width();
        int f = ((cursor.y())-margen.top())/celdaSz.height();
        if(f>-1 && f<f_ && c>-1 && c<c_){                                         //prevenir errores de calculo de pocos pixeles
            cout<<"Corresponde a la celda "<<c<<","<<f<<endl;
            ((celda*)layPrincipal_->itemAtPosition(f,c)->widget())->cambiarTipo(pintar_);
        }
    }else{
        cout<<"Clic fuera del area"<<endl;
    }
}

void MainWindow::mousePressEvent(QMouseEvent* E){
    if(E->button()==Qt::LeftButton){
        pintar_=true;
    }else if(E->button()==Qt::RightButton){
        pintar_=false;
    }
}
