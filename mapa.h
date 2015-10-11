#ifndef MAPA_H
#define MAPA_H
#include <QWidget>
#include <QGridLayout>
#include "celda.h"
#include <QProgressBar>
#include <QPixmap>
#include <QFileDialog>
#include <fstream>

class mapa : public QWidget{
    Q_OBJECT
public:
    mapa(int,int,QProgressBar*,int,QWidget* parent = 0);
    mapa(std::ifstream*,QProgressBar*,QWidget* parent = 0);
    void resizeEvent(QResizeEvent* );
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent* );
    int getFilas();
    int getColumnas();
    void actualizarEsteMapa(int);
    void guardar(std::ofstream*);
    void pintar();
private:
    QGridLayout* layMapa_;                      //layout del mapa
    int c_;                                     //columnas de la matriz
    int f_;                                     //filas de la matriz
    bool pintar_;                               //true=añadir obstaculos, false=borrar obstaculos
    QPixmap* pixSuelo_;
    QPixmap* pixMuro_;
    QProgressBar* barra_;
    int factor_;
    QFileDialog* dialogoAbrir_;
    QString*     rutaArchivo_;
    void cambiarCeldaEn(int,int,bool);
private slots:
    void limpiarMapa();
signals:
    void actualizarBarra(int);
};



#endif // MAPA_H
