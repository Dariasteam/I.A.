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




    filas = new QSpinBox();
    columnas = new QSpinBox();

    filas->setValue(10);
    columnas->setValue(10);

    connect(filas,SIGNAL(valueChanged(int)),this,SLOT(actualizarF(int)));
    connect(columnas,SIGNAL(valueChanged(int)),this,SLOT(actualizarC(int)));

    filas->setAcceptDrops(true);
    layPrincipal_->addWidget(filas);
    layPrincipal_->addWidget(columnas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actualizarC(int c){
    mapa* aux = new mapa(widMapa_->getFilas(),c);
    layPrincipal_->replaceWidget(widMapa_,aux);
    delete widMapa_;
    widMapa_=aux;
}

void MainWindow::actualizarF(int f){
    mapa* aux = new mapa(f,widMapa_->getColumnas());
    layPrincipal_->replaceWidget(widMapa_,aux);
    delete widMapa_;
    widMapa_=aux;
}

//Calcular si la pulsacion se produce en el interior de la ventana


