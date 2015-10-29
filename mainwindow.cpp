#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "celda.h"
#include "mapa.h"

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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

//INICIALIZACION DE LA VENTANA Y SUS WIDGETS

    ui->setupUi(this);
    widPrincipal_   = new QWidget(this);
    layPrincipal_   = new QBoxLayout(QBoxLayout::TopToBottom,widPrincipal_);
    barraProgreso_  = new QProgressBar(this);
    layPrincipal_->addWidget(barraProgreso_);
    widPrincipal_->setLayout(layPrincipal_);
    mapas_ = new QTabWidget(this);

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

//INICIALIZACION DE LOS PIX

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
    pincel_ = 5;

    widMapa_ = new mapa(10,10,barraProgreso_,0,0,0,0,graficosTerrenos_,((QWidget*)this));

//INCIALIZACIÓN DEL DOCK Y SU CONTENIDO

    dockIzquierda_      =   new QDockWidget("Herramientas",this);
    dockIzquierda_->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockIzquierda_->setFeatures(QDockWidget::DockWidgetMovable);
    panelDesplegable_ = new QToolBox(dockIzquierda_);
    dockIzquierda_->setWidget(panelDesplegable_);

    QWidget* opcionesMapa = new QWidget(panelDesplegable_);
    QWidget* opcionesAlgoritmo = new QWidget(panelDesplegable_);
    QWidget* opcionesEstadistica = new QWidget(panelDesplegable_);

    panelDesplegable_->addItem(opcionesMapa,       "Mapa");
    panelDesplegable_->addItem(opcionesAlgoritmo,  "Agente");
    panelDesplegable_->addItem(opcionesEstadistica,"Estadística");

    layOpcionesMapa_ = new QGridLayout(opcionesMapa);
    layOpcionesAlgoritmo_ = new QBoxLayout(QBoxLayout::TopToBottom,opcionesAlgoritmo);
    layOpcionesEstadistica_ = new QBoxLayout(QBoxLayout::TopToBottom,opcionesEstadistica);
    layOpcionesMapa_->setSizeConstraint(QLayout::SetFixedSize);
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
    botonSimular_->setCheckable(true);
    botonRastro_ = new QPushButton("Rastro todos",this);
    botonRastro_->setCheckable(true);
    botonMemoria_=new QPushButton("Memoria todos",this);
    botonMemoria_->setCheckable(true);

    QBoxLayout* layDropBot = new QBoxLayout(QBoxLayout::LeftToRight,NULL);
    layDropBot->setSizeConstraint(QBoxLayout::SetFixedSize);

    layOpcionesAlgoritmo_->addLayout(layDropBot);

    dropbot* drop = new dropbot(this);
    drop->setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));

    layDropBot->addWidget(drop);
    layDropBot->addWidget(new QLabel("Arrastra y suelta\npara añadir\nun agente\n\nVelocidad:"));

    velocidadSlider_ = new QSlider(Qt::Horizontal,contenedor);
    velocidadSlider_->setRange(1,100);
    velocidadSlider_->setInvertedControls(true);
    velocidadSlider_->setInvertedAppearance(true);
    velocidadSlider_->setValue(50);

    scrollAgentes_ = new QScrollArea(this);
    scrollAgentes_->setWidget(contenedor);
    scrollAgentes_->setWidgetResizable(true);

    layDropBot->setSizeConstraint(QLayout::SetMinimumSize);
    layOpcionesAlgoritmo_->setSizeConstraint(QLayout::SetMinimumSize);
    layScrollAgentes_->setSizeConstraint(QLayout::SetFixedSize);
    layScrollAgentes_->setMargin(0);
    drop->setMinimumSize(110,110);

    layOpcionesAlgoritmo_->addWidget(velocidadSlider_);
    layOpcionesAlgoritmo_->addWidget(scrollAgentes_);
    layOpcionesAlgoritmo_->addWidget(botonRastro_);
    layOpcionesAlgoritmo_->addWidget(botonMemoria_);
    layOpcionesAlgoritmo_->addWidget(botonSimular_);

    connect(botonSimular_,&QAbstractButton::clicked,this,&MainWindow::onSimular);
    connect(botonMemoria_,&QAbstractButton::clicked,this,&MainWindow::actualizarMemoria);
    connect(velocidadSlider_,SIGNAL(valueChanged(int)),this,SLOT(velocidad(int)));

//INICIALIZACION DEL PANEL "ESTADISTICA"



//TOOL BAR

    QToolBar * barra = new QToolBar("ToolBar",this);
    barra->setFloatable(false);
    barra->setAllowedAreas(Qt::AllToolBarAreas);
    this->addToolBar(barra);

    QLabel* pincelesTool = new QLabel("Terrenos: ",this);
    barra->addWidget(pincelesTool);

    muro_ = new QAction(QIcon(graficosTerrenos_[muro]),"muro",this);
    rojo_ = new QAction(QIcon(graficosTerrenos_[rojo]),"rojo",this);
    suelo_ = new QAction(QIcon(graficosTerrenos_[suelo]),"suelo",this);
    metal_ = new QAction(QIcon(graficosTerrenos_[metal]),"metal",this);
    rejilla_ = new QAction(QIcon(graficosTerrenos_[rejilla]),"rejilla",this);
    tierra_ = new QAction(QIcon(graficosTerrenos_[tierra]),"tierra",this);
    nuclear_ = new QAction(QIcon(graficosTerrenos_[nuclear]),"nuclear",this);

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

    ultimoAction_ = suelo_;

    auto changeultimoAction_ = [&] (QAction* action) {
        ultimoAction_->setChecked(false);
        ultimoAction_ = action;
        ultimoAction_->setChecked(true);
    };

    connect(muro_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(muro_); this->setPincel(muro);}));
    connect(rojo_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(rojo_); this->setPincel(rojo);}));
    connect(suelo_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(suelo_); this->setPincel(suelo);}));
    connect(metal_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(metal_); this->setPincel(metal);}));
    connect(rejilla_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(rejilla_); this->setPincel(rejilla);}));
    connect(tierra_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(tierra_); this->setPincel(tierra);}));
    connect(nuclear_, &QAction::triggered, this, ([=] (void) { changeultimoAction_(nuclear_); this->setPincel(nuclear);}));

//OPERACIONES FINALES

    zoomSlider_ = new QSlider(Qt::Horizontal,this);
    zoomSlider_->setRange(1,100);
    zoomSlider_->setValue(1);

    connect(zoomSlider_,&QAbstractSlider::valueChanged,this,&MainWindow::zoomSobre);
    connect(botonRastro_,&QAbstractButton::clicked,this,&MainWindow::actualizarRastro);
    mapas_->addTab(widMapa_,"Escenario");

    mapas_->setMaximumSize(widMapa_->width(),widMapa_->height()+35);

    layPrincipal_->addWidget(mapas_);
    layPrincipal_->addWidget(zoomSlider_);
    setCentralWidget(widPrincipal_);

    memoria_ = new mapa(widMapa_->getFilas(),widMapa_->getColumnas(),graficosTerrenos_,this);
    mapas_->addTab(memoria_,"Memoria");
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

void MainWindow::actualizarMapa(){
    cout<<"Generando nuevo mapa"<<endl;
    mapa* aux = new mapa(spinFilas_->value(),spinColumnas_->value(),barraProgreso_,
                   editoresTerreno_[0].valorAnterior_,
                   editoresTerreno_[1].valorAnterior_,
                   editoresTerreno_[2].valorAnterior_,
                   editoresTerreno_[3].valorAnterior_,graficosTerrenos_,this);
    operacionesActualizacion(aux);
}

void MainWindow::onAbrir(){
    rutaArchivo_= new QString(dialogoAbrir_->getOpenFileName(this,"Abrir Mapa","","*.map"));
    ifstream fich;
    if(rutaArchivo_->contains(".map")){
        fich.open(rutaArchivo_->toStdString().c_str(), ios::in);
        if(fich.is_open()){
            mapa* aux = new mapa(&fich,barraProgreso_,graficosTerrenos_,this);
            fich.close();
            actGuardar_->setEnabled(true);
            setWindowTitle("I.A.[*] - "+*rutaArchivo_);
            operacionesActualizacion(aux);
        }else{
            QMessageBox* error = new QMessageBox();
            error->setText("No se ha podido abrir el archivo");
            error->show();
        }
    }else{
        cout<<"El fichero no es un archivo .map"<<endl;
    }
}

void MainWindow::operacionesActualizacion(mapa* aux){
    while(mapas_->count()>0){
        mapas_->removeTab(0);
    }
    mapas_->insertTab(0,aux,"Escenario");
    widMapa_ = ((mapa*)mapas_->widget(0));
    velocidadSlider_->setValue(50);
    zoomSlider_->setValue(1);
    botonRastro_->setChecked(false);
    botonSimular_->setChecked(false);
    botonMemoria_->setChecked(false);
    while(agentes_.count()>0){
        while(!agentes_.at(0)->terminar()){
        }
        agentes_.removeFirst();
    }

    while(!layScrollAgentes_->isEmpty()){
        delete layScrollAgentes_->takeAt(0);
    }
    int i=0;
    botonSimular_->setText("Simular");
    mapas_->setMaximumSize(widMapa_->width(),widMapa_->height()+35);
    delete memoria_;
    memoria_ = new mapa(widMapa_->getFilas(),widMapa_->getColumnas(),graficosTerrenos_,this);
    mapas_->addTab(memoria_,"Memoria");
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
        guardar(&fich);
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


void MainWindow::guardar(ofstream* fich){
    int f = widMapa_->getFilas();
    int c = widMapa_->getColumnas();
    *fich<<f<<" "<<c<<endl;
    for(int i=0;i<f;i++){
        for(int j=0;j<c;j++){
            *fich<<widMapa_->getCelda(i,j)->tipo_<<" ";
        }
        *fich<<endl;
    }
}

void MainWindow::setPincel(short tipo){
    pincel_ = tipo;
}

void MainWindow::pintar(){
    double anchoMapa  = ((mapa*)mapas_->widget(0))->width();
    double altoMapa   = ((mapa*)mapas_->widget(0))->height();
    double ratonX = (mousePos_.x());
    double ratonY = (mousePos_.y());
    int f = widMapa_->getFilas();
    int c = widMapa_->getColumnas();
    if((ratonX > 0) && (mousePos_.x() < anchoMapa) && (ratonY > 0) && (mousePos_.y() < altoMapa)){
        double xCelda = anchoMapa  / c;
        double yCelda = altoMapa   / f;
        double c = ratonX / xCelda;
        double f = ratonY / yCelda;
        int fila    = (int)(f);
        int columna = (int)(c);
        if(fila>-1 && fila<f && columna>-1 && columna<c){
            widMapa_->sustituirCelda(fila,columna,pincel_);
        }
    }
}


void MainWindow::onSimular(){
    if(botonSimular_->text()=="Simular"){
        botonSimular_->setText("Simulando");
        for(int i=0;i<agentes_.size();i++){
            agentes_.at(i)->start();
        }
    }else{
        botonSimular_->setText("Simular");
        for(int i=0;i<agentes_.size();i++){
            agentes_.at(i)->pause();
        }
    }
}

void MainWindow::velocidad(int i){
    for(int j=0;j<agentes_.size();j++){
        agentes_.at(j)->setVelocidad(i);
    }
}

void MainWindow::actualizarRastro(bool b){
    for(int i=0;i<agentes_.size();i++){
        agentes_.at(i)->setRastro(b);
    }
}

void MainWindow::actualizarSeguir(int id){
    int i=0;
    while(i<agentes_.size()){
        if(i!=id){
            agentes_.at(i)->setSeguir(false);
        }
        i++;
    }
}

void MainWindow::actualizarMemoria(bool b){
    for(int i=0;i<agentes_.size();i++){
        agentes_.at(i)->setMemoria(b);
    }
}

void MainWindow::addAgente(QPointF posReal){
    QPoint P = widMapa_->getFilaColumna(posReal);
    QGraphicsPixmapItem* gPix = (widMapa_->pintarPixmap(P.y(),P.x(),&graficosAgente_[1]));
    agente* aux = new agente(P.x(),P.y(),widMapa_->getEscala()*32,agentes_.size(),gPix,graficosAgente_,widMapa_,memoria_,this);
    layScrollAgentes_->addWidget(aux);
    agentes_.push_back(aux);
    aux->detontante();
    aux->setVelocidad(velocidadSlider_->value());
    if(botonSimular_->isChecked()){
        aux->start();
    }
}

void MainWindow::movioMouse(QPointF mousePos){
    mousePos_ = mousePos;
    pintar();
}

void MainWindow::zoomSobre(int z){
    for(int i=0;i<mapas_->count();i++){
        ((mapa*)mapas_->widget(i))->zoom(z);
    }
}
