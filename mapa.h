#ifndef MAPA_H
#define MAPA_H

#include "graphicsmapa.h"
#include "agente.h"
#include "celda.h"

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

class agente;

struct celda{
    short                                   tipo_;
    QGraphicsPixmapItem*                    pix_;
    agente*                                 agente_;
};

struct dirYPesos{
    short                                   direccion_[4];  //0 Arriba, 1 Abajo, 2 Derecha, 3 Izquierda
};

class mapa : public QWidget{
    Q_OBJECT
public:
    explicit mapa(int,int,QProgressBar*,short,short,short,short,QWidget* parent = 0);
    explicit mapa(std::ifstream*,QProgressBar*, QWidget* parent = 0);
    int getFilas();
    int getColumnas();
    void guardar(std::ofstream*);
    void pintar();
    void cambiarCeldaEn(int,int,bool);
    int pos(int,int);
    void operacionesConstruccion(int,int,QProgressBar*);
    QGraphicsPixmapItem* pintarPixmap(double,double,QPixmap*);
    void sustituirCelda(double,double,short);
    void cambiarTipoPincel(short);
    dirYPesos escanearEntorno(int x, int y);
    void addAgente(QPointF);
private:
    celda*                                  matrizMapa_;
    QPixmap*                                graficosTerrenos_;
    QPixmap*                                graficosAgente_;
    short                                   pincel_;
    QBoxLayout*                             layMapa_;
    int                                     c_;
    int                                     f_;
    QPointF                                 mousePos_;
    QProgressBar*                           barra_;
    int                                     factor_;
    QFileDialog*                            dialogoAbrir_;
    QString*                                rutaArchivo_;
    graphicsMapa *                          escena_;
    QGraphicsView*                          view_;
    double                                  escala_;
    QList<agente*>                          agentes_;
    QList<agente*>                          movimientosActuales_;
    QList<QGraphicsPixmapItem*>             pixAgentes_;
    double                                  ultimoZoom_;
    QSlider*                                zoomSlider_;
    int                                     direccionMovimiento_;    //0 Arriba, 1 Abajo, 2 Derecha, 3 Izquierda
    int                                     idAgente_;
    QTimer*                                 tiempo_;
    bool                                    simulando_;
    QWidget*                                parent_;
public slots:
    void movioMouse(QPointF);
    void zoom(int);
    void movimientoTempo();
    void startSimulacion();
    void agentePideMovimiento(agente*,int,int);
signals:
    void actualizarBarra(int);
};



#endif // MAPA_H
