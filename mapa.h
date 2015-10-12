#ifndef MAPA_H
#define MAPA_H
#include <QWidget>
#include <QGridLayout>
#include "celda.h"
#include <QProgressBar>
#include <QPixmap>
#include <QFileDialog>
#include <fstream>
#include <QSize>
#include <QTimer>
#include "robot.h"

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
    void cambiarCeldaEn(int,int,bool);

    void quitarRobotDeActual();
    void ponerRobotEn(int,int);
    void moverRobot(int,int);


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

    QPixmap* pixRobot_;
    int ira_; // i and j actual position of the robot
    int jra_;
    robot* robot_;

    QTimer* timer_;

    QSize   celdaSize_;

    int camino_;

private slots:
    void limpiarMapa();
    void gestorRobot();
    void reiniciarRobot();
signals:
    void actualizarBarra(int);
};



#endif // MAPA_H
