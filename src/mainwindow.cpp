#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setMinimumSize(800,600);

    widMap_ = new Map(10,10,0,0,0,0,this);
    ui->setupUi(this);
    ui->map_layout->addWidget(widMap_);

    QButtonGroup * buttonGroup = new QButtonGroup(this);
    QPushButton * muro_ = new QPushButton(QIcon(QPixmap(":/recursos/muro.png")),"Muro",this);
    QPushButton * rojo_ = new QPushButton(QIcon(QPixmap(":/recursos/rojo.png")),"Rojo",this);
    QPushButton * suelo_ = new QPushButton(QIcon(QPixmap(":/recursos/suelo.png")),"Suelo",this);
    QPushButton * metal_ = new QPushButton(QIcon(QPixmap(":/recursos/metal.png")),"Metal",this);
    QPushButton * rejilla_ = new QPushButton(QIcon(QPixmap(":/recursos/rejilla.png")),"Rejilla",this);
    QPushButton * tierra_ = new QPushButton(QIcon(QPixmap(":/recursos/tierra.png")),"Tierra",this);

    muro_->setCheckable(true);
    rojo_->setCheckable(true);
    suelo_->setCheckable(true);
    metal_->setCheckable(true);
    rejilla_->setCheckable(true);
    tierra_->setCheckable(true);

    buttonGroup->addButton(muro_);
    buttonGroup->addButton(rojo_);
    buttonGroup->addButton(suelo_);
    buttonGroup->addButton(metal_);
    buttonGroup->addButton(rejilla_);
    buttonGroup->addButton(tierra_);

    connect(buttonGroup,SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(changePencil(QAbstractButton*)));

    ui->toolBar->addWidget(muro_);
    ui->toolBar->addWidget(rojo_);
    ui->toolBar->addWidget(suelo_);
    ui->toolBar->addWidget(metal_);
    ui->toolBar->addWidget(rejilla_);
    ui->toolBar->addWidget(tierra_);

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

void MainWindow::changePencil(QAbstractButton * button) {
    if (button->text() == "Muro") {
        widMap_->setPencil(Wall);
    }
    else if (button->text() == "Rojo") {
        widMap_->setPencil(RedTile);
    }
    else if (button->text() == "Suelo") {
        widMap_->setPencil(Ground);
    }
    else if (button->text() == "Metal") {
        widMap_->setPencil(Metal);
    }
    else if (button->text() == "Rejilla") {
        widMap_->setPencil(Network);
    }
    else if (button->text() == "Tierra") {
        widMap_->setPencil(Land);
    }
}
