#ifndef MAPA_H
#define MAPA_H
#include <QWidget>
#include <QGridLayout>
#include "celda.h"

class mapa : public QWidget{
    Q_OBJECT
public:
    explicit mapa(QWidget *parent = 0);
    mapa(int,int);
    void resizeEvent(QResizeEvent* );
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent* );
    int getFilas();
    int getColumnas();
private:
    QGridLayout* layMapa_;                      //layout del mapa
    int c_;                                     //columnas de la matriz
    int f_;                                     //filas de la matriz
    bool pintar_ =true;                         //true=añadir obstaculos, false=borrar obstaculos
private slots:
    //void actualizarF(int);
    //void actualizarC(int);
};

#endif // MAPA_H
