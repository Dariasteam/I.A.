#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setMinimumSize(800,600);

    widMap_ = new Map(10,10,0,0,0,0,this);
    ui->setupUi(this);
    ui->map_layout->addWidget(widMap_);

    QAction * muro_ = new QAction(QIcon(QPixmap(":/recursos/muro.png")),"Muro",this);
    QAction * rojo_ = new QAction(QIcon(QPixmap(":/recursos/rojo.png")),"Rojo",this);
    QAction * suelo_ = new QAction(QIcon(QPixmap(":/recursos/suelo.png")),"Suelo",this);
    QAction * metal_ = new QAction(QIcon(QPixmap(":/recursos/metal.png")),"Metal",this);
    QAction * rejilla_ = new QAction(QIcon(QPixmap(":/recursos/rejilla.png")),"Rejilla",this);
    QAction * tierra_ = new QAction(QIcon(QPixmap(":/recursos/tierra.png")),"Tierra",this);

    active_ = muro_;
    muro_->setCheckable(true);
    rojo_->setCheckable(true);
    suelo_->setCheckable(true);
    metal_->setCheckable(true);
    rejilla_->setCheckable(true);
    tierra_->setCheckable(true);

    auto changeActive = [&] (QAction * action) {
        active_->setChecked(false);
        active_ = action;
        active_->setChecked(true);
    };
    connect(muro_, &QAction::triggered, this, ([=] (void) { changeActive(muro_); widMap_->setPencil(Wall);}));
    connect(rojo_, &QAction::triggered, this, ([=] (void) { changeActive(rojo_); widMap_->setPencil(RedTile);}));
    connect(suelo_, &QAction::triggered, this, ([=] (void) { changeActive(suelo_); widMap_->setPencil(Ground);}));
    connect(metal_, &QAction::triggered, this, ([=] (void) { changeActive(metal_); widMap_->setPencil(Metal);}));
    connect(rejilla_, &QAction::triggered, this, ([=] (void) { changeActive(rejilla_); widMap_->setPencil(Network);}));
    connect(tierra_, &QAction::triggered, this, ([=] (void) { changeActive(tierra_); widMap_->setPencil(Land);}));

    ui->toolBar->addAction(muro_);
    ui->toolBar->addAction(rojo_);
    ui->toolBar->addAction(suelo_);
    ui->toolBar->addAction(metal_);
    ui->toolBar->addAction(rejilla_);
    ui->toolBar->addAction(tierra_);

    ui->progressBar->hide();
    ui->actionGuardar->setDisabled(true);


    QToolBox * toolBox = new QToolBox(ui->dockRight);
    ui->dockRight->setWidget(toolBox);

    mapOptions_ = new MapOptions(toolBox);
    toolBox->addItem(mapOptions_, "Mapa");

    connect(ui->zoom,SIGNAL(valueChanged(int)),widMap_,SLOT(makeZoom(int)));
    connect(mapOptions_,SIGNAL(onSpawn()),this,SLOT(onUpdateMap()));
    connect(ui->actionAbrir,SIGNAL(triggered(bool)),this,SLOT(onOpen()));
    connect(ui->actionGuardar,SIGNAL(triggered(bool)),this,SLOT(onSave()));
    connect(ui->actionGuardar_Como ,SIGNAL(triggered(bool)),this,SLOT(onSaveAs()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onUpdateMap(){
    Map* aux;
    disconnect(ui->zoom,SIGNAL(valueChanged(int)),widMap_,SLOT(makeZoom(int)));
    cout<<"Generando nuevo mapa"<<endl;
    aux = new Map(mapOptions_->cols(),mapOptions_->rows(),
                   mapOptions_->muro(),
                   mapOptions_->tierra(),
                   mapOptions_->rejilla(),
                   mapOptions_->metal(), this);
    ui->map_layout->replaceWidget(widMap_, aux);
    delete widMap_;
    widMap_=aux;
    connect(ui->zoom,SIGNAL(valueChanged(int)),widMap_,SLOT(makeZoom(int)));
}

void MainWindow::onOpen(){
    rutaArchivo_= new QString(dialogoAbrir_->getOpenFileName(this,"Abrir Mapa","","*.map"));
    ifstream fich;
    if(rutaArchivo_->contains(".map")){
        fich.open(rutaArchivo_->toStdString().c_str(), ios::in);
        if(fich.is_open()){
            Map* aux;
            aux = new Map(&fich,this);
            ui->map_layout->replaceWidget(widMap_,aux);
            delete widMap_;
            widMap_=aux;
            fich.close();
            ui->actionGuardar->setEnabled(true);
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

void MainWindow::onSaveAs(){
    rutaArchivo_=new QString(dialogoAbrir_->getSaveFileName(this,"Guardar Mapa","mapa","*.map"));
    if(!rutaArchivo_->contains(".map")){
        rutaArchivo_ = new QString(*rutaArchivo_+".map");
    }
    onSave();
}

void MainWindow::onSave(){
    ofstream fich;
    cout<<"Voy a guardar en"<<rutaArchivo_->toStdString()<<endl;
    fich.open(rutaArchivo_->toStdString().c_str(), std::fstream::out | std::fstream::trunc);
    if(fich.is_open()){
        if (widMap_->save(&fich))
            setWindowModified(false);
        fich.close();
        ui->actionGuardar->setEnabled(true);
    }else{
        QMessageBox* error = new QMessageBox();
        error->setText("No se ha podido abrir el archivo");
        error->show();
    }
}
