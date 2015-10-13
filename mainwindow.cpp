#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QBoxLayout>
#include "celda.h"
#include <QMouseEvent>
#include <QPoint>
#include <iostream>
#include <QMargins>
#include <QSpinBox>
#include <QBoxLayout>
#include <QRect>
#include "mapa.h"
#include <QPushButton>
#include <QProgressBar>
#include <QCheckBox>
#include <QLabel>
#include <fstream>
#include <QMessageBox>
#include <QSizePolicy>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsItem>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    setMouseTracking(true);
    ui->setupUi(this);
    widPrincipal_ = new QWidget(this);
    layPrincipal_ = new QBoxLayout(QBoxLayout::TopToBottom,widPrincipal_);
    layMenu_      = new QGridLayout();
    barraProgreso_ = new QProgressBar(this);

    layPrincipal_->addWidget(barraProgreso_);
    widMapa_ = new mapa(10,10,barraProgreso_,0,this);


    widPrincipal_->setLayout(layPrincipal_);
    layPrincipal_->addWidget(widMapa_);
    layPrincipal_->addLayout(layMenu_);

    QPushButton* boton_ = new QPushButton("Generar");
    boton_->setShortcut(Qt::Key_Enter);

    //actAyudaAcerca_->setShortcut((QKeySequence(Qt::CTRL + Qt::Key_H)));




    setCentralWidget(widPrincipal_);

    botonClear_ = new QPushButton("Clear");

    layMenu_->addWidget(boton_,0,0);
    layMenu_->addWidget(botonClear_,0,1);

    checkAleatorio_ = new QCheckBox("Obstáculos aleatorios");
    QLabel* textoFactor = new QLabel("Factor de aleatoriedad [1,50]",this);


    spinFilas_ = new QSpinBox();
    spinColumnas_ = new QSpinBox();
    spinFactor_ = new QSpinBox();

    spinFilas_->setValue(10);
    spinColumnas_->setValue(10);
    spinFactor_->setValue(1);

    spinFilas_->setMaximum(1000);
    spinColumnas_->setMaximum(1000);
    spinFactor_->setMaximum(50);
    spinFactor_->setMinimum(1);
    spinFactor_->setEnabled(false);

    connect(checkAleatorio_,SIGNAL(clicked(bool)),spinFactor_,SLOT(setEnabled(bool)));
    connect(boton_,SIGNAL(clicked(bool)),this,SLOT(actualizarMapa()));
    connect(botonClear_, SIGNAL(clicked()), widMapa_, SLOT(limpiarMapa()));

    layMenu_->addWidget(spinFilas_,1,0);
    layMenu_->addWidget(spinColumnas_,2,0);
    layMenu_->addWidget(checkAleatorio_,0,3);
    layMenu_->addWidget(textoFactor,1,1);
    layMenu_->addWidget(spinFactor_,2,1);


    menuBar_ = new QMenuBar(this);
    mnuArchivo_ = new QMenu("Archivo",this);

    actCargar_ = new QAction("Abrir",mnuArchivo_);
    actGuardar_ = new QAction("Guardar",mnuArchivo_);
    actGuardar_->setDisabled(true);
    actGuardarComo_ = new QAction("Guardar como",mnuArchivo_);

    mnuArchivo_->addAction(actCargar_);
    mnuArchivo_->addAction(actGuardar_);
    mnuArchivo_->addAction(actGuardarComo_);

    connect(actCargar_,SIGNAL(triggered(bool)),this,SLOT(onAbrir()));
    connect(actGuardar_,SIGNAL(triggered(bool)),this,SLOT(onGuardar()));
    connect(actGuardarComo_,SIGNAL(triggered(bool)),this,SLOT(onGuardarComo()));
    menuBar_->addMenu(mnuArchivo_);
    setMenuBar(menuBar_);
    this->setWindowTitle("I.A.[*]");

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e){
}

void MainWindow::actualizarMapa(){
    mapa* aux;
    if((widMapa_->getFilas()==spinFilas_->value()) && (widMapa_->getColumnas()==spinColumnas_->value())){
        cout<<"actualizando el mapa actual"<<endl;
        //valor del spinFactor * bool check para enviar 0 si este está desactivado
        widMapa_->actualizarEsteMapa(spinFactor_->value()*checkAleatorio_->isChecked());
    }else{
        cout<<"Generando nuevo mapa"<<endl;
        aux = new mapa(spinFilas_->value(),spinColumnas_->value(),barraProgreso_,spinFactor_->value()*checkAleatorio_->isChecked(),this);
        layPrincipal_->replaceWidget(widMapa_,aux);
        delete widMapa_;
        widMapa_=aux;
        actualizarConnects();
    }
}


void MainWindow::actualizarConnects(){
    connect(botonClear_, SIGNAL(clicked()), widMapa_, SLOT(limpiarMapa()));
}

void MainWindow::onAbrir(){
    rutaArchivo_= new QString(dialogoAbrir_->getOpenFileName(this,"Abrir Mapa","","*.map"));
    ifstream fich;
    if(rutaArchivo_->contains(".map")){
        fich.open(rutaArchivo_->toStdString().c_str(), ios::in);
        if(fich.is_open()){
            mapa* aux;
            aux = new mapa(&fich,barraProgreso_,this);
            layPrincipal_->replaceWidget(widMapa_,aux);
            delete widMapa_;
            widMapa_=aux;
            actualizarConnects();
            fich.close();
            actGuardar_->setEnabled(true);
            this->setWindowTitle("I.A.[*] - "+*rutaArchivo_);
        }else{
            QMessageBox* error = new QMessageBox();
            error->setText("No se ha podido abrir el archivo");
            error->show();
        }
    }else{
        cout<<"El fichero no es un archivo .map"<<endl;
    }
}

void MainWindow::onGuardarComo(){
    rutaArchivo_=new QString(dialogoAbrir_->getSaveFileName(this,"Guardar Mapa","mapa","*.map"));
    if(!rutaArchivo_->contains(".map")){
        rutaArchivo_ = new QString(*rutaArchivo_+".map");
    }
    onGuardar();
}

void MainWindow::onGuardar(){
    ofstream fich;
    cout<<"Voy a guardar en"<<rutaArchivo_->toStdString()<<endl;
    fich.open(rutaArchivo_->toStdString().c_str(), std::fstream::out | std::fstream::trunc);
    if(fich.is_open()){
        widMapa_->guardar(&fich);
        fich.close();
        actGuardar_->setEnabled(true);
    }else{
        QMessageBox* error = new QMessageBox();
        error->setText("No se ha podido abrir el archivo");
        error->show();
    }
}

void MainWindow::actualizarTitulo(bool b){
    if(b){
        this->setWindowModified(true);
    }else{
        this->setWindowModified(false);
    }
}




