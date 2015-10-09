#ifndef MAPA_H
#define MAPA_H
#include <QWidget>
#include <QGridLayout>
#include "celda.h"
#include <QProgressBar>
#include <QPixmap>

class mapa : public QWidget{
    Q_OBJECT
public:
    mapa(int,int,QProgressBar*,int,QWidget* parent = 0);
    void resizeEvent(QResizeEvent* );
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent* );
    int getFilas();
    int getColumnas();

    void pintar();

private:
    QBoxLayout* layBox_;
    QGridLayout* layMapa_;                      //layout del mapa
    int c_;                                     //columnas de la matriz
    int f_;                                     //filas de la matriz
    bool pintar_ =true;                         //true=añadir obstaculos, false=borrar obstaculos
    QPixmap* pixSuelo_;
    QPixmap* pixMuro_;

    void CambiarCeldaEn(int,int,bool);
private slots:
    void limpiarMapa();
    signals:
    void actualizarBarra(int);
};



#endif // MAPA_H
