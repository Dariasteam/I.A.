#ifndef mapa_H
#define mapa_H

#include "graphicsmapa.h"
#include "pixmapitem.h"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include <fstream>
#include <QScrollBar>
#include <mutex>


class MainWindow;

enum tile {
    nuclear,
    suelo,
    metal,
    rejilla,
    tierra,
    rojo,
    muro,
};

class agente;

struct celda{
    short                                   tipo_;
    QGraphicsPixmapItem*                    pix_;
    agente*                                 agente_;
    int                                     x_;
    int                                     y_;
    void imprimir();
};

enum direcciones {
    arriba,
    abajo,
    derecha,
    izquierda,
};

struct objetivo{
    celda* cel_;
    bool   marcado_;
};

class mapa : public QGraphicsView{
public:
    mapa(int f,int c,QProgressBar*,short,short,short,short,QPixmap*,QWidget* parent);
    mapa(std::ifstream*,QProgressBar*,QPixmap*,QWidget* parent);
    mapa(int f,int c, QPixmap*,QWidget* parent);
    void operacionesConstruccion(QProgressBar*);
    int getFilas();
    int getColumnas();
    celda* getCelda(int,int);
    void setCelda(int,int,short);
    int pos(int,int);
    void guardar(std::ofstream*);
    QGraphicsPixmapItem* pintarPixmap(double,double,QPixmap*);
    void sustituirCelda(double,double,short);
    short* escanearEntorno(int x, int y);
    QPoint getFilaColumna(QPointF);
    double getEscala();
    int getCoste(celda* pos, celda* objetivo);
    std::mutex                              mu_;
    QList<objetivo*>                        objetivos_;
private:
    celda*                                  mapa_;
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
