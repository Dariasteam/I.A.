#ifndef MAPA_H
#define MAPA_H

#include "graphicsmapa.h"
#include "celda.h"
#include "agente.h"

#include <QWidget>
#include <QBoxLayout>
#include <QProgressBar>
#include <QPixmap>
#include <QFileDialog>
#include <fstream>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSlider>

struct celda{
    short                                   tipo_;
    QGraphicsPixmapItem*                    pix_;
    agente*                                 agente_;
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

enum direcciones {
    arriba,
    abajo,
    derecha,
    izquierda,

};

class mapa : public QGraphicsView{
    Q_OBJECT
public:
    explicit mapa(int,int,QProgressBar*,short,short,short,short, QBoxLayout*, QWidget* parent = 0);
    explicit mapa(std::ifstream*,QProgressBar*, QBoxLayout*, QWidget* parent = 0);
    int getFilas();
    int getColumnas();
    void guardar(std::ofstream*);
    void pintar();
    void cambiarCeldaEn(int,int,bool);
    int pos(int,int);
    QPoint getFilaColumna(QPointF);
    void operacionesConstruccion(int,int,QProgressBar*);
    QGraphicsPixmapItem* pintarPixmap(double,double,QPixmap*);
    void sustituirCelda(double,double,short);
    void setPincel(short);
    void addAgente(QPointF);
    void seguirAgente(double x, double y);
    bool stopSimulacion();
    short* escanearEntorno(int x, int y);
private:
    celda*                                  matrizMapa_;
    QPixmap*                                graficosTerrenos_;
    QPixmap*                                graficosAgente_;
    short                                   pincel_;
    int                                     c_;
    int                                     f_;
    QPointF                                 mousePos_;
    QProgressBar*                           barra_;
    int                                     factor_;
    QBoxLayout*                             layScrollAgentes_;
    QFileDialog*                            dialogoAbrir_;
    QString*                                rutaArchivo_;
    graphicsMapa*                           escena_;
    double                                  escala_;
    QList<agente*>                          agentes_;
    double                                  ultimoZoom_;
    bool                                    simulando_;
    bool                                    rastroTodos_;
    QWidget*                                parent_;
public slots:
    void movioMouse(QPointF);
    void zoom(int);
    void velocidad(int);
    void startSimulacion();
    void actualizarRastro();
    void actualizarSeguir(int);
    //void agentePideMovimiento(agente*,int,int,QGraphicsPixmapItem*,bool,bool);
signals:
    void actualizarBarra(int);
};



#endif // MAPA_H
