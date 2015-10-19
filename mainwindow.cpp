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
#include <QDockWidget>
#include <QToolBox>
#include <QSpacerItem>
#include <unistd.h>
#include <QThread>
#include <thread>
#include <QFrame>
#include <QPixmap>
#include <QToolBar>
#include <QButtonGroup>

#include <QPushButton>

using namespace std;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

//INICIALIZACION DE LA VENTANA Y SUS WIDGETS

    ui->setupUi(this);
    widPrincipal_   = new QWidget(this);
    layPrincipal_   = new QBoxLayout(QBoxLayout::TopToBottom,widPrincipal_);
    barraProgreso_  = new QProgressBar(this);
    layPrincipal_->addWidget(barraProgreso_);
    widMapa_ = new mapa(10,10,barraProgreso_,0,0,0,0,this);
    widPrincipal_->setLayout(layPrincipal_);
    layPrincipal_->addWidget(widMapa_);

    setWindowTitle("I.A.[*]");
    layout()->setSizeConstraint(QLayout::SetFixedSize);

//INICIALIZACION DE LOS MENUS

    menuBar_        = new QMenuBar(this);
    mnuArchivo_     = new QMenu("Archivo",this);
    actCargar_      = new QAction("Abrir",mnuArchivo_);
    actGuardar_     = new QAction("Guardar",mnuArchivo_);
    actGuardarComo_ = new QAction("Guardar como",mnuArchivo_);

    actGuardar_->setDisabled(true);

    mnuArchivo_->addAction(actCargar_);
    mnuArchivo_->addAction(actGuardar_);
    mnuArchivo_->addAction(actGuardarComo_);

    connect(actCargar_,     SIGNAL(triggered(bool)),this,SLOT(onAbrir()));
    connect(actGuardar_,    SIGNAL(triggered(bool)),this,SLOT(onGuardar()));
    connect(actGuardarComo_,SIGNAL(triggered(bool)),this,SLOT(onGuardarComo()));

    menuBar_->addMenu(mnuArchivo_);
    setMenuBar(menuBar_);

//INCIALIZACIÓN DEL DOCK Y SU CONTENIDO

    dockIzquierda_      =   new QDockWidget("Herramientas",this);
    dockIzquierda_->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockIzquierda_->setFeatures(QDockWidget::DockWidgetMovable);
    panelDesplegable_ = new QToolBox(dockIzquierda_);
    dockIzquierda_->setWidget(panelDesplegable_);

    QWidget* opcionesMapa = new QWidget(panelDesplegable_);
    QWidget* opcionesAlgoritmo = new QWidget(panelDesplegable_);

    panelDesplegable_->addItem(opcionesMapa,"Mapa");
    panelDesplegable_->addItem(opcionesAlgoritmo,"Algoritmo");

    layOpcionesMapa_ = new QGridLayout(opcionesMapa);
    layOpcionesAlgoritmo_ = new QGridLayout(opcionesAlgoritmo);

    dockIzquierda_->setMinimumWidth(150);
    dockIzquierda_->setMaximumWidth(150);

    addDockWidget(Qt::RightDockWidgetArea,dockIzquierda_);

//INCIALIZACION DEL PANEL "MAPA"

    QFrame* linea   = new QFrame();
    linea->setFrameShape(QFrame::HLine);
    QFrame* linea2   = new QFrame();
    linea2->setFrameShape(QFrame::HLine);


    spinFilas_      = new QSpinBox();
    spinColumnas_   = new QSpinBox();
    spinFilas_      ->setValue(10);
    spinColumnas_   ->setValue(10);
    spinFilas_      ->setMaximum(10000);
    spinColumnas_   ->setMaximum(10000);

    layOpcionesMapa_->addWidget(new QLabel("Dimensiones: ",opcionesMapa),0,0,1,0);
    layOpcionesMapa_->addWidget(new QLabel("filas",opcionesMapa),1,0);
    layOpcionesMapa_->addWidget(spinFilas_,1,1);
    layOpcionesMapa_->addWidget(new QLabel("columnas",opcionesMapa),2,0);
    layOpcionesMapa_->addWidget(spinColumnas_,2,1);
    layOpcionesMapa_->addWidget(linea,3,0,1,0);
    layOpcionesMapa_->addWidget(new QLabel("Terrenos: "),4,0,1,0);
    editoresTerreno_= new menuTerreno[4];
    crearLabelSlider("Muro",5,0);
    crearLabelSlider("Metal",7,1);
    crearLabelSlider("Rejilla",9,2);
    crearLabelSlider("Tierra",11,3);
    layOpcionesMapa_->addWidget(linea2,13,0,1,0);
    botonGenerar_   = new QPushButton("Generar");
    layOpcionesMapa_->addWidget(botonGenerar_,14,0,1,2);
    layOpcionesMapa_->setSizeConstraint(QLayout::SetFixedSize);

    connect(botonGenerar_,SIGNAL(clicked(bool)),this,SLOT(actualizarMapa()));

//TOOL BAR



    QToolBar * barra = new QToolBar("suso",this);
    barra->setFloatable(false);
    barra->setAllowedAreas(Qt::AllToolBarAreas);
    this->addToolBar(barra);

    muro_ = new QAction(QIcon(QPixmap("../I.A./recursos/muro.png")),"muro",this);
    rojo_ = new QAction(QIcon(QPixmap("../I.A./recursos/rojo.png")),"rojo",this);
    suelo_ = new QAction(QIcon(QPixmap("../I.A./recursos/suelo.png")),"suelo",this);
    metal_ = new QAction(QIcon(QPixmap("../I.A./recursos/metal.png")),"metal",this);
    rejilla_ = new QAction(QIcon(QPixmap("../I.A./recursos/rejilla.png")),"rejilla",this);
    tierra_ = new QAction(QIcon(QPixmap("../I.A./recursos/tierra.png")),"Tierra",this);


    QLabel* pincelesTool = new QLabel("Terrenos: ",this);
    barra->addWidget(pincelesTool);
   // QButtonGroup * ese = new QButtonGroup(barra);

   // ese->addButton(new QPushButton(QIcon(QPixmap("../I.A./recursos/muro.png")),"",this),0);


    muro_->setCheckable(true);
    rojo_->setCheckable(true);
    suelo_->setCheckable(true);
    metal_->setCheckable(true);
    rejilla_->setCheckable(true);
    tierra_->setCheckable(true);

    barra->addAction(muro_);
    barra->addAction(rojo_);
    barra->addAction(suelo_);
    barra->addAction(metal_);
    barra->addAction(rejilla_);
    barra->addAction(tierra_);

    connect(muro_,SIGNAL(triggered()),this,SLOT(cambiarPincelAMuro()));
    connect(rojo_,SIGNAL(triggered()),this,SLOT(cambiarPincelARojo()));
    connect(suelo_,SIGNAL(triggered()),this,SLOT(cambiarPincelASuelo()));
    connect(metal_,SIGNAL(triggered()),this,SLOT(cambiarPincelAMetal()));
    connect(rejilla_,SIGNAL(triggered()),this,SLOT(cambiarPincelARejilla()));
    connect(tierra_,SIGNAL(triggered()),this,SLOT(cambiarPincelATierra()));



//OPERACIONES FINALES

    setCentralWidget(widPrincipal_);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::crearLabelSlider(QString nombre, int posLay, int i){
    editoresTerreno_[i].slider_ = new QSlider(Qt::Horizontal);
    editoresTerreno_[i].slider_->setRange(0,50);
    editoresTerreno_[i].slider_->setValue(0);
    editoresTerreno_[i].label_ = new QLabel("0");
    connect(editoresTerreno_[i].slider_,SIGNAL(valueChanged(int)),this,SLOT(actualizarSliders()));
    connect(editoresTerreno_[i].slider_,SIGNAL(valueChanged(int)),editoresTerreno_[i].label_,SLOT(setNum(int)));
    layOpcionesMapa_->addWidget(new QLabel(nombre),posLay,0);
    layOpcionesMapa_->addWidget(editoresTerreno_[i].label_,posLay,1);
    layOpcionesMapa_->addWidget(editoresTerreno_[i].slider_,posLay+1,0,1,0);
    editoresTerreno_[i].valorAnterior_ = 0;
}

void MainWindow::actualizarSliders(){
    int i=0;
    while(i<4 && editoresTerreno_[i].valorAnterior_==editoresTerreno_[i].slider_->value()){
        i++;
    }
    if(i<4){
        if((editoresTerreno_[0].slider_->value() +
            editoresTerreno_[1].slider_->value() +
            editoresTerreno_[2].slider_->value() +
            editoresTerreno_[3].slider_->value())  > 50){

            float factorReduccion =
            50 - (editoresTerreno_[0].slider_->value()>0)*editoresTerreno_[0].slider_->value() +
            (editoresTerreno_[1].slider_->value()>0)*editoresTerreno_[1].slider_->value() +
            (editoresTerreno_[2].slider_->value()>0)*editoresTerreno_[2].slider_->value() +
            (editoresTerreno_[3].slider_->value()>0)*editoresTerreno_[3].slider_->value();

            factorReduccion = factorReduccion /
            (((editoresTerreno_[0].slider_->value()>0) * (0!=i)) +
             ((editoresTerreno_[1].slider_->value()>0) * (1!=i)) +
             ((editoresTerreno_[2].slider_->value()>0) * (2!=i)) +
             ((editoresTerreno_[3].slider_->value()>0) * (3!=i)));

            if(factorReduccion>0 && factorReduccion<INFINITY){
                for(int j=0;j<4;j++){
                    if(j!=i && editoresTerreno_[j].valorAnterior_ >
                       fabs(editoresTerreno_[j].valorAnterior_-(factorReduccion*editoresTerreno_[j].valorAnterior_>0))
                       && editoresTerreno_[j].valorAnterior_>0){
                            editoresTerreno_[j].valorAnterior_ = fabs(editoresTerreno_[j].valorAnterior_-
                                                                     (factorReduccion*editoresTerreno_[j].valorAnterior_>0));
                            editoresTerreno_[j].slider_->setValue(editoresTerreno_[j].valorAnterior_);
                            editoresTerreno_[0].label_->setNum(editoresTerreno_[0].valorAnterior_);
                    }
                }
            }
        }
        editoresTerreno_[i].valorAnterior_ = editoresTerreno_[i].slider_->value();
        editoresTerreno_[0].label_->setNum(editoresTerreno_[0].valorAnterior_);
        editoresTerreno_[1].label_->setNum(editoresTerreno_[1].valorAnterior_);
        editoresTerreno_[2].label_->setNum(editoresTerreno_[2].valorAnterior_);
        editoresTerreno_[3].label_->setNum(editoresTerreno_[3].valorAnterior_);
    }
}

void MainWindow::resizeEvent(QResizeEvent*){
}

void MainWindow::actualizarMapa(){
    mapa* aux;
    cout<<"Generando nuevo mapa"<<endl;
    aux = new mapa(spinFilas_->value(),spinColumnas_->value(),barraProgreso_,
                   editoresTerreno_[0].valorAnterior_,
                   editoresTerreno_[1].valorAnterior_,
                   editoresTerreno_[2].valorAnterior_,
                   editoresTerreno_[3].valorAnterior_,this);
    layPrincipal_->replaceWidget(widMapa_,aux);
    delete widMapa_;
    widMapa_=aux;
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


void MainWindow::cambiarPincelAMuro()
{
    widMapa_->cambiarTipoPincel(6);
    muro_->setChecked(true);
    suelo_->setChecked(false);
    tierra_->setChecked(false);
    rojo_->setChecked(false);
    rejilla_->setChecked(false);
    metal_->setChecked(false);
}


void MainWindow::cambiarPincelARojo()
{
    widMapa_->cambiarTipoPincel(5);

    muro_->setChecked(false);
    suelo_->setChecked(false);
    tierra_->setChecked(false);
    rojo_->setChecked(true);
    rejilla_->setChecked(false);
    metal_->setChecked(false);
}


void MainWindow::cambiarPincelASuelo()
{
    widMapa_->cambiarTipoPincel(4);


    muro_->setChecked(false);
    suelo_->setChecked(true);
    tierra_->setChecked(false);
    rojo_->setChecked(false);
    rejilla_->setChecked(false);
    metal_->setChecked(false);
}


void MainWindow::cambiarPincelAMetal()
{
    widMapa_->cambiarTipoPincel(3);

    muro_->setChecked(false);
    suelo_->setChecked(false);
    tierra_->setChecked(false);
    rojo_->setChecked(false);
    rejilla_->setChecked(false);
    metal_->setChecked(true);
}

void MainWindow::cambiarPincelATierra()
{
    widMapa_->cambiarTipoPincel(2);


    muro_->setChecked(false);
    suelo_->setChecked(false);
    tierra_->setChecked(true);
    rojo_->setChecked(false);
    rejilla_->setChecked(false);
    metal_->setChecked(false);
}


void MainWindow::cambiarPincelARejilla()
{
    widMapa_->cambiarTipoPincel(1);


    muro_->setChecked(false);
    suelo_->setChecked(false);
    tierra_->setChecked(false);
    rojo_->setChecked(false);
    rejilla_->setChecked(true);
    metal_->setChecked(false);
}



