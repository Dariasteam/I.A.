#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QBoxLayout>
#include "mapa.h"
#include <QSpinBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void actualizarMapa(int,int);
private:
    Ui::MainWindow *ui;
    QWidget* widPrincipal_;                     //widget principal de la ventana
    QBoxLayout*  layPrincipal_;                 //layout principal de la ventana
    mapa*    widMapa_;
    QSpinBox* filas;
    QSpinBox* columnas;
private slots:
    void actualizarF(int);
    void actualizarC(int);
};

#endif // MAINWINDOW_H
