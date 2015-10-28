#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapa.h"
#include "dropbot.h"
#include "agente.h"

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
#include <QTabWidget>
#include <QSlider>

namespace Ui {
    class MainWindow;
}

struct menuTerreno{
    QSlider*                            slider_;
    QLabel*                             label_;
    short                               valorAnterior_;
};

enum tile {
    nuclear,
    rejilla,
    tierra,
    metal,
    suelo,
    rojo,
    muro,
};

class agente;
class mapa;

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void actualizarTitulo(bool);
    void crearLabelSlider(QString,int,int);
    void guardar(std::ofstream*);
    void pintar();
    void addAgente(QPointF);
    void operacionesActualizacion(mapa*);
private:
    Ui::MainWindow*                     ui;
    QWidget*                            widPrincipal_;
    QBoxLayout*                         layPrincipal_;
    mapa*                               widMapa_;
    mapa*                               memoria_;
    QSpinBox*                           spinFilas_;
    QSpinBox*                           spinColumnas_;
    QProgressBar*                       barraProgreso_;
    QPushButton*                        botonGenerar_;
    QPushButton*                        botonSimular_;
    QPushButton*                        botonRastro_;
    QPushButton*                        botonMemoria_;
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
    QSlider*                            velocidadSlider_;
    QTabWidget*                         mapas_;
    QPixmap*                            graficosTerrenos_;
    QPixmap*                            graficosAgente_;
    short                               pincel_;
    QList<agente*>                      agentes_;
    QPointF                             mousePos_;
private slots:
    void actualizarMapa();
    void actualizarSliders();
    void onAbrir();
    void onGuardar();
    void onGuardarComo();
    void onSimular();
    void setPincel(short);
    void velocidad(int);
    void actualizarRastro(bool);
    void actualizarMemoria(bool);
    void zoomSobre(int);
public slots:
    void actualizarSeguir(int);
    void movioMouse(QPointF);
};

#endif // MAINWINDOW_H
