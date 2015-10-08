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

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setMouseTracking(true);
    ui->setupUi(this);
    widPrincipal_ = new QWidget();
    layPrincipal_ = new QBoxLayout(QBoxLayout::TopToBottom);
    widMapa_ = new mapa(10,10);

    this->setCentralWidget(widPrincipal_);
    widPrincipal_->setLayout(layPrincipal_);
    layPrincipal_->addWidget(widMapa_);

    QPushButton* boton_ = new QPushButton("Generar");
    layPrincipal_->addWidget(boton_);



    spinFilas_ = new QSpinBox();
    spinColumnas_ = new QSpinBox();

    spinFilas_->setValue(10);
    spinColumnas_->setValue(10);

    spinFilas_->setMaximum(500);
    spinColumnas_->setMaximum(500);


    connect(boton_,SIGNAL(clicked(bool)),this,SLOT(actualizarMapa()));

    spinFilas_->setAcceptDrops(true);
    layPrincipal_->addWidget(spinFilas_);
    layPrincipal_->addWidget(spinColumnas_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actualizarMapa(){
    mapa* aux = new mapa(spinFilas_->value(),spinColumnas_->value());
    layPrincipal_->replaceWidget(widMapa_,aux);
    delete widMapa_;
    widMapa_=aux;
}
//Calcular si la pulsacion se produce en el interior de la ventana


