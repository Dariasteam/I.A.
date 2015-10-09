#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QBoxLayout>
#include "mapa.h"
#include <QSpinBox>
#include <QProgressBar>
#include <QCheckBox>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QWidget* widPrincipal_;                     //widget principal de la ventana
    QBoxLayout*  layPrincipal_;                 //layout principal de la ventana
    QGridLayout* layMenu_;                      //layout para los menus del mapa
    mapa*    widMapa_;
    QSpinBox* spinFilas_;
    QSpinBox* spinColumnas_;
    QSpinBox* spinFactor_;
    QCheckBox* checkAleatorio_;
    QProgressBar* barraProgreso_;

    QPushButton * botonClear_;
private slots:
    void actualizarMapa();
};

#endif // MAINWINDOW_H
