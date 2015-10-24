#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapa.h"
#include "dropbot.h"
//#include "agente.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QBoxLayout>
#include <QSpinBox>
#include <QProgressBar>
#include <QCheckBox>
#include <QPushButton>
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
    QSlider*                            slider_;
    QLabel*                             label_;
    short                               valorAnterior_;
};

class agente;

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void actualizarTitulo(bool);
    void crearLabelSlider(QString,int,int);
    //void addAgente();
    void actualizarAgentes();
    void actualizarSeguir(int);
protected:
    void resizeEvent(QResizeEvent *);
private:
    Ui::MainWindow*                     ui;
    QWidget*                            widPrincipal_;
    QBoxLayout*                         layPrincipal_;
    mapa*                               widMapa_;
    QSpinBox*                           spinFilas_;
    QSpinBox*                           spinColumnas_;
    QProgressBar*                       barraProgreso_;
    QPushButton*                        botonGenerar_;
    QPushButton*                        botonSimular_;
    QMenuBar*                           menuBar_;
    QMenu*                              mnuArchivo_;
    QAction*                            actGuardar_;
    QAction*                            actGuardarComo_;
    QAction*                            actCargar_;
    QString*                            rutaArchivo_;
    QFileDialog*                        dialogoAbrir_;
    QBoxLayout*                         layScrollAgentes_;
    QScrollArea*                        scrollAgentes_;
    QDockWidget*                        dockIzquierda_;
    QToolBox*                           panelDesplegable_;
    QGridLayout*                        layOpcionesMapa_;
    QBoxLayout*                         layOpcionesAlgoritmo_;
    QBoxLayout*                         layOpcionesEstadistica_;
    menuTerreno*                        editoresTerreno_;
    QAction*                            muro_;
    QAction*                            rojo_;
    QAction*                            suelo_;
    QAction*                            metal_;
    QAction*                            rejilla_;
    QAction*                            tierra_;
    QAction*                            nuclear_;
    QAction*                            ultimoAction_;
    QSlider*                            zoomSlider_;
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
    void cambiarPincelANuclear();
    void onSimular();
public slots:
};

#endif // MAINWINDOW_H
