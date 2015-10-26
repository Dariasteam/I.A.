#ifndef mapa_H
#define mapa_H

#include "agente.h"
#include "graphicsmapa.h"
#include "mainwindow.h"

#include <QGraphicsView>
#include <QProgressBar>
#include <fstream>
#include <QScrollBar>


class MainWindow;
class agente;

struct celda{
    short                                   tipo_;
    QGraphicsPixmapItem*                    pix_;
    agente*                                 agente_;
};

enum direcciones {
    arriba,
    abajo,
    derecha,
    izquierda,
};

class mapa : public QGraphicsView{
public:
    mapa(int f,int c,QProgressBar*,short,short,short,short,QPixmap*,QWidget* parent);
    mapa(std::ifstream*,QProgressBar*,QPixmap*,QWidget* parent);
    void operacionesConstruccion(QProgressBar*);
    int getFilas();
    int getColumnas();
    short getCelda(int,int);
    int pos(int,int);
    void guardar(std::ofstream*);
    QGraphicsPixmapItem* pintarPixmap(double,double,QPixmap*);
    void sustituirCelda(double,double,short);
    short* escanearEntorno(int x, int y);
    QPoint getFilaColumna(QPointF);
    double getEscala();
    void enfocar(double x, double y);
private:
    celda*                                  mapaMapa_;
    QPixmap*                                graficosTerrenos_;
    short                                   pincel_;
    int                                     c_;
    int                                     f_;
    QProgressBar*                           barra_;
    int                                     factor_;
    graphicsMapa*                           escena_;
    double                                  escala_;
    QWidget*                                parent_;
    double                                  ultimoZoom_;
public slots:
    void zoom(int);
signals:
    void actualizarBarra(int);
};

#endif // mapa_H
