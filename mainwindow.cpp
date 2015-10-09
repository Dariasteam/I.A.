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
#include <QSizePolicy>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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

    setCentralWidget(widPrincipal_);
    widPrincipal_->setLayout(layPrincipal_);
    layPrincipal_->addWidget(widMapa_);
    layPrincipal_->addLayout(layMenu_);

    QPushButton* boton_ = new QPushButton("Generar");
    boton_->setShortcut(Qt::Key_Enter);

    //actAyudaAcerca_->setShortcut((QKeySequence(Qt::CTRL + Qt::Key_H)));

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

    spinFilas_->setMaximum(500);
    spinColumnas_->setMaximum(500);
    spinFactor_->setMaximum(50);
    spinFactor_->setMinimum(1);
    spinFactor_->setEnabled(false);



    connect(checkAleatorio_,SIGNAL(clicked(bool)),spinFactor_,SLOT(setEnabled(bool)));
    connect(boton_,SIGNAL(clicked(bool)),this,SLOT(actualizarMapa()));
    connect(botonClear_, SIGNAL(clicked()), widMapa_, SLOT(limpiarMapa()));



    spinFilas_->setAcceptDrops(true);
    layMenu_->addWidget(spinFilas_,1,0);
    layMenu_->addWidget(spinColumnas_,2,0);
    layMenu_->addWidget(checkAleatorio_,0,3);
    layMenu_->addWidget(textoFactor,1,1);
    layMenu_->addWidget(spinFactor_,2,1);


    menuBar_ = new QMenuBar(this);
    mnuArchivo_ = new QMenu("Archivo",this);


    menuBar_->addMenu(mnuArchivo_);

    setMenuBar(menuBar_);

    cout << "Construimos";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
}




void MainWindow::actualizarMapa(){
    mapa* aux;

    if(checkAleatorio_->isChecked()){
        aux = new mapa(spinFilas_->value(),spinColumnas_->value(),barraProgreso_,spinFactor_->value(),this);
    }else{
        aux = new mapa(spinFilas_->value(),spinColumnas_->value(),barraProgreso_,0,this);
    }
    layPrincipal_->replaceWidget(widMapa_,aux);
    delete widMapa_;
    widMapa_=aux;
    connect(botonClear_, SIGNAL(clicked()), widMapa_, SLOT(limpiarMapa()));
}

