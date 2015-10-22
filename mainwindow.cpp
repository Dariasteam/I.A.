#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "celda.h"
#include "mapa.h"
#include "ficha.h"
#include "agente.h"

#include <iostream>
#include <QLabel>
#include <fstream>
#include <QMessageBox>
#include <QFrame>
#include <QPixmap>
#include <QToolBar>
#include <QGroupBox>

using namespace std;

class agente;
class ficha;

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
    QWidget* opcionesEstadistica = new QWidget(panelDesplegable_);

    panelDesplegable_->addItem(opcionesMapa,"Mapa");
    panelDesplegable_->addItem(opcionesAlgoritmo,"Agente");
    panelDesplegable_->addItem(opcionesEstadistica,"Estadística");

    layOpcionesMapa_ = new QGridLayout(opcionesMapa);
    layOpcionesAlgoritmo_ = new QBoxLayout(QBoxLayout::TopToBottom,opcionesAlgoritmo);
    layOpcionesEstadistica_ = new QBoxLayout(QBoxLayout::TopToBottom,opcionesEstadistica);

    dockIzquierda_->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
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

//INICIALIZACION DEL PANEL "AGENTE"

    QWidget* contenedor = new QWidget(this);
    layScrollAgentes_ = new QBoxLayout(QBoxLayout::TopToBottom,contenedor);

    botonSimular_ = new QPushButton("Simular",this);

    QBoxLayout* layDropBot = new QBoxLayout(QBoxLayout::LeftToRight,NULL);

    layOpcionesAlgoritmo_->addLayout(layDropBot);

    dropbot* drop = new dropbot(this);
    drop->setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));

    layDropBot->addWidget(drop);
    layDropBot->addWidget(new QLabel("Arrastra y suelta\npara añadir\nun agente"));
    layOpcionesAlgoritmo_->setSizeConstraint(QLayout::SetMaximumSize);

    scrollAgentes_ = new QScrollArea(this);
    scrollAgentes_->setWidget(contenedor);
    scrollAgentes_->setWidgetResizable(true);
    layScrollAgentes_->setSizeConstraint(QLayout::SetFixedSize);

    layOpcionesAlgoritmo_->addWidget(scrollAgentes_);
    layOpcionesAlgoritmo_->addWidget(botonSimular_);

    connect(botonSimular_,SIGNAL(clicked(bool)),this,SLOT(onSimular()));

//INICIALIZACION DEL PANEL "ESTADISTICA"



//TOOL BAR

    QToolBar * barra = new QToolBar("ToolBar",this);
    barra->setFloatable(false);
    barra->setAllowedAreas(Qt::AllToolBarAreas);
    this->addToolBar(barra);

    muro_ = new QAction(QIcon(QPixmap("../I.A./recursos/muro.png")),"muro",this);
    rojo_ = new QAction(QIcon(QPixmap("../I.A./recursos/rojo.png")),"rojo",this);
    suelo_ = new QAction(QIcon(QPixmap("../I.A./recursos/suelo.png")),"suelo",this);
    metal_ = new QAction(QIcon(QPixmap("../I.A./recursos/metal.png")),"metal",this);
    rejilla_ = new QAction(QIcon(QPixmap("../I.A./recursos/rejilla.png")),"rejilla",this);
    tierra_ = new QAction(QIcon(QPixmap("../I.A./recursos/tierra.png")),"Tierra",this);
    nuclear_ = new QAction(QIcon(QPixmap("../I.A./recursos/nuclear.png")),"Nuclear",this);
    ultimoAction_ = suelo_;

    QLabel* pincelesTool = new QLabel("Terrenos: ",this);
    barra->addWidget(pincelesTool);

    muro_->setCheckable(true);
    rojo_->setCheckable(true);
    suelo_->setCheckable(true);
    metal_->setCheckable(true);
    rejilla_->setCheckable(true);
    tierra_->setCheckable(true);
    nuclear_->setCheckable(true);

    barra->addAction(muro_);
    barra->addAction(rojo_);
    barra->addAction(suelo_);
    barra->addAction(metal_);
    barra->addAction(rejilla_);
    barra->addAction(tierra_);
    barra->addSeparator();
    barra->addAction(nuclear_);

    connect(muro_,SIGNAL(triggered()),this,SLOT(cambiarPincelAMuro()));
    connect(rojo_,SIGNAL(triggered()),this,SLOT(cambiarPincelARojo()));
    connect(suelo_,SIGNAL(triggered()),this,SLOT(cambiarPincelASuelo()));
    connect(metal_,SIGNAL(triggered()),this,SLOT(cambiarPincelAMetal()));
    connect(rejilla_,SIGNAL(triggered()),this,SLOT(cambiarPincelARejilla()));
    connect(tierra_,SIGNAL(triggered()),this,SLOT(cambiarPincelATierra()));
    connect(nuclear_,SIGNAL(triggered(bool)),SLOT(cambiarPincelANuclear()));

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

void MainWindow::actualizarAgentes(){
    while(!layScrollAgentes_->isEmpty()){
        delete layScrollAgentes_->takeAt(0);
    }
    botonSimular_->setText("Simular");
    layScrollAgentes_->update();
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
    actualizarAgentes();
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
            actualizarAgentes();
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

void MainWindow::actualizarTitulo(bool b){this->setWindowModified(true);
    if(b){
        this->setWindowModified(true);
    }else{
        this->setWindowModified(false);
    }
}


void MainWindow::cambiarPincelAMuro(){
    ultimoAction_->setChecked(false);
    widMapa_->cambiarTipoPincel(6);
    ultimoAction_ = muro_;
}


void MainWindow::cambiarPincelARojo(){
    ultimoAction_->setChecked(false);
    widMapa_->cambiarTipoPincel(5);
    ultimoAction_=rojo_;
}


void MainWindow::cambiarPincelASuelo(){
    ultimoAction_->setChecked(false);
    widMapa_->cambiarTipoPincel(4);
    ultimoAction_=suelo_;
}

void MainWindow::cambiarPincelANuclear(){
    ultimoAction_->setCheckable(false);
    widMapa_->cambiarTipoPincel(0);
    ultimoAction_=suelo_;
}

void MainWindow::cambiarPincelAMetal(){
    ultimoAction_->setChecked(false);
    widMapa_->cambiarTipoPincel(3);
    ultimoAction_=metal_;
}

void MainWindow::cambiarPincelATierra(){
    ultimoAction_->setChecked(false);
    widMapa_->cambiarTipoPincel(2);
    ultimoAction_=tierra_;
}


void MainWindow::cambiarPincelARejilla(){
    ultimoAction_->setChecked(false);
    widMapa_->cambiarTipoPincel(1);
    ultimoAction_=rejilla_;
}

void MainWindow::addAgente(agente* a, int id){
    ficha* aux = new ficha(QString("Agente ")+QString::fromStdString(std::to_string(id)),a,NULL);
    layScrollAgentes_ ->addWidget(aux);
}

void MainWindow::onSimular(){
    if(botonSimular_->text()=="Simular"){
        botonSimular_->setText("Pausa");
    }else{
        botonSimular_->setText("Simular");
    }
    widMapa_->startSimulacion();
}
