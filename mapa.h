#ifndef MAPA_H
#define MAPA_H
#include <QWidget>
#include <QBoxLayout>
#include "celda.h"
#include <QProgressBar>
#include <QPixmap>
#include <QFileDialog>
#include <fstream>
#include <QSize>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSlider>
#include "graphicsmapa.h"

namespace Ui {
    class mapa;
}

struct celda{
    short tipo_;
    QGraphicsPixmapItem* pix_;
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
    void moverPixmapItem(short dir,QGraphicsPixmapItem* pix);
private:
    Ui::mapa* ui;
    celda*              matrizMapa_;
    QPixmap*            terrenos_;
    short               pincel_;
    QGraphicsPixmapItem* robot_;
    QBoxLayout* layMapa_;                      //layout del mapa
    int c_;                                     //columnas de la matriz
    int f_;                                     //filas de la matriz
    bool pintar_;                               //true=añadir obstaculos, false=borrar obstaculos
    QPixmap* pixSuelo_;
    QPixmap* pixMuro_;
    QPointF     mousePos_;
    QProgressBar* barra_;
    int factor_;
    QFileDialog* dialogoAbrir_;
    QString*     rutaArchivo_;
    graphicsMapa *  escena_;
    QGraphicsView*  view_;
    double          escala_;
    bool            tiempo_;
    int             ultimoZoom_;
    QSlider*        zoomSlider_;
public slots:
    void movioMouse(QPointF);
    void zoom(int);
signals:
    void actualizarBarra(int);
};



#endif // MAPA_H
