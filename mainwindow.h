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
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QScrollArea>
#include <QDockWidget>
#include <QToolBox>
#include <QList>

namespace Ui {
    class MainWindow;
}

struct menuTerreno{
    QSlider*   slider_;
    QLabel*    label_;
    short      valorAnterior_;
};

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void actualizarTitulo(bool);
    void crearLabelSlider(QString,int,int);
protected:
    void resizeEvent(QResizeEvent *);
private:
    Ui::MainWindow *ui;
    QWidget* widPrincipal_;                     //widget principal de la ventana
    QBoxLayout*  layPrincipal_;                 //layout principal de la ventana
    mapa*    widMapa_;
    QSpinBox* spinFilas_;
    QSpinBox* spinColumnas_;
    QProgressBar* barraProgreso_;
    QPushButton* botonGenerar_;
    QMenuBar*    menuBar_;
    QMenu*       mnuArchivo_;
    QAction*     actGuardar_;
    QAction*     actGuardarComo_;
    QAction*     actCargar_;
    QString*     rutaArchivo_;
    QFileDialog* dialogoAbrir_;
    QScrollArea* areaScroll_;
    QDockWidget* dockIzquierda_;
    QToolBox*    panelDesplegable_;
    QGridLayout* layOpcionesMapa_;
    QGridLayout* layOpcionesAlgoritmo_;
    menuTerreno*       editoresTerreno_;
    //unsigned short*       valoresTerreno_;
    //QList<QSlider*>     slidersTerreno_;

    QAction * muro_;
    QAction * rojo_;
    QAction * suelo_;
    QAction * metal_;
    QAction * rejilla_;
    QAction * tierra_;

private slots:
    void actualizarMapa();
    void onAbrir();
    void onGuardar();
    void onGuardarComo();
    void actualizarSliders();
    void cambiarPincelAMuro();
    void cambiarPincelARojo();
    void cambiarPincelASuelo();
    void cambiarPincelARejilla();
    void cambiarPincelAMetal();
    void cambiarPincelATierra();
};

#endif // MAINWINDOW_H
