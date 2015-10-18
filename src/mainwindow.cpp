#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setMinimumSize(800,600);

    widMap_ = new Map(10,10,0,0,0,0,this);
    ui->setupUi(this);
    ui->map_layout->addWidget(widMap_);

    ui->progressBar->hide();
    ui->actionGuardar->setDisabled(true);

    connect(ui->zoom,SIGNAL(valueChanged(int)),widMap_,SLOT(makeZoom(int)));
    connect(ui->generar,SIGNAL(clicked(bool)),this,SLOT(onUpdateMap()));
    connect(ui->actionAbrir,SIGNAL(triggered(bool)),this,SLOT(onOpen()));
    connect(ui->actionGuardar,SIGNAL(triggered(bool)),this,SLOT(onSave()));
    connect(ui->actionGuardar_Como ,SIGNAL(triggered(bool)),this,SLOT(onSaveAs()));

    connect(ui->slider_metal,SIGNAL(valueChanged(int)),this,SLOT(onSliderUpdate(CellTile)));
    connect(ui->slider_metal,SIGNAL(valueChanged(int)),ui->metal,SLOT(setNum(int)));
}

void MainWindow::onUpdateMap(){
    Map* aux;
    cout<<"Generando nuevo mapa"<<endl;
    aux = new Map(ui->cols->value(),ui->rows->value(),
                   ui->slider_muro->value(),
                   ui->slider_tierra->value(),
                   ui->slider_rejilla->value(),
                   ui->slider_metal->value(), this);
    ui->map_layout->replaceWidget(widMap_, aux);
    delete widMap_;
    widMap_=aux;
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


//    editoresTerreno_[i].slider_ = new QSlider(Qt::Horizontal);
//    editoresTerreno_[i].slider_->setRange(0,50);
//    editoresTerreno_[i].slider_->setValue(0);
//    editoresTerreno_[i].label_ = new QLabel("0");
//    connect(editoresTerreno_[i].slider_,SIGNAL(valueChanged(int)),this,SLOT(actualizarSliders()));
//    connect(editoresTerreno_[i].slider_,SIGNAL(valueChanged(int)),editoresTerreno_[i].label_,SLOT(setNum(int)));
//    layOpcionesMapa_->addWidget(new QLabel(nombre),posLay,0);
//    layOpcionesMapa_->addWidget(editoresTerreno_[i].label_,posLay,1);
//    layOpcionesMapa_->addWidget(editoresTerreno_[i].slider_,posLay+1,0,1,0);
//    editoresTerreno_[i].valorAnterior_ = 0;

void MainWindow::onSliderUpdate(CellTile){ // TODO :: CellTile -> Void
    return;
//    int i=0;
//    while(i<4 && editoresTerreno_[i].valorAnterior_==editoresTerreno_[i].slider_->value()){
//        i++;
//    }
//    if(i<4){
//        // if sum . map x.slider_->value editoresTerreno_  > 50
//        if((editoresTerreno_[0].slider_->value() +
//            editoresTerreno_[1].slider_->value() +
//            editoresTerreno_[2].slider_->value() +
//            editoresTerreno_[3].slider_->value())  > 50){

//            // 50 - sum . map x.slider_->value editoresTerreno_
//            float factorReduccion =
//            editoresTerreno_[0].slider_->value() +
//            editoresTerreno_[1].slider_->value() +
//            editoresTerreno_[2].slider_->value() +
//            editoresTerreno_[3].slider_->value() - 50;

//            // factorReduccion / (sum . map (if x.slider_->value > 0 then 1 else 0)-1)
//            factorReduccion = factorReduccion /
//            (((editoresTerreno_[0].slider_->value()>0)) +
//             ((editoresTerreno_[1].slider_->value()>0)) +
//             ((editoresTerreno_[2].slider_->value()>0)) +
//             ((editoresTerreno_[3].slider_->value()>0))-1);

//            if(factorReduccion>0 && factorReduccion<INFINITY){
//                // map ()
//                for(int j=0;j<4;j++){
//                    if(j!=i && editoresTerreno_[j].valorAnterior_ >
//                       fabs(editoresTerreno_[j].valorAnterior_-(factorReduccion*editoresTerreno_[j].valorAnterior_>0))
//                       && editoresTerreno_[j].valorAnterior_>0){

//                            editoresTerreno_[j].valorAnterior_ = fabs(editoresTerreno_[j].valorAnterior_-
//                                                                     (factorReduccion*editoresTerreno_[j].valorAnterior_>0));
//                            editoresTerreno_[j].slider_->setValue(editoresTerreno_[j].valorAnterior_);
//                            editoresTerreno_[0].label_->setNum(editoresTerreno_[0].valorAnterior_);
//                    }
//                }
//            }
//        }
//        editoresTerreno_[i].valorAnterior_ = editoresTerreno_[i].slider_->value();
//        editoresTerreno_[0].label_->setNum(editoresTerreno_[0].valorAnterior_);
//        editoresTerreno_[1].label_->setNum(editoresTerreno_[1].valorAnterior_);
//        editoresTerreno_[2].label_->setNum(editoresTerreno_[2].valorAnterior_);
//        editoresTerreno_[3].label_->setNum(editoresTerreno_[3].valorAnterior_);
//    }
}
