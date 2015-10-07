#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QBoxLayout>
#include "celda.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    widPrincipal_ = new QWidget();
    layPrincipal_ = new QGridLayout();
    for(int i=0;i<30;i++){
        for(int j=0;j<30;j++){
            celda* aux = new celda(i,j);
            layPrincipal_->addWidget(aux,i,j);
        }
    }
    widPrincipal_->setLayout(layPrincipal_);
    this->setCentralWidget(widPrincipal_);
    layPrincipal_->setSpacing(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
