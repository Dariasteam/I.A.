#ifndef MAPA_H
#define MAPA_H
#include <QWidget>
#include <QGridLayout>
#include "celda.h"
#include <QProgressBar>

class mapa : public QWidget{
    Q_OBJECT
public:
    explicit mapa(QWidget *parent = 0);
    mapa(int,int,QProgressBar*);
    void resizeEvent(QResizeEvent* );
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent* );
    int getFilas();
    int getColumnas();
private:
    QBoxLayout* layBox_;
    QGridLayout* layMapa_;                      //layout del mapa
    int c_;                                     //columnas de la matriz
    int f_;                                     //filas de la matriz
    bool pintar_ =true;                         //true=añadir obstaculos, false=borrar obstaculos
private slots:
    signals:
    void actualizarBarra(int);

};



#endif // MAPA_H
