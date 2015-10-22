#ifndef AGENTE_H
#define AGENTE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <thread>
#include <QPixmap>
#include <QColor>

class agente : public QWidget{


public:
    explicit agente(int x, int y, int tiempoMov_, int id, QWidget *parent = 0);
    void movimiento();
    void start();
    void finMovimiento();
    void setColor(QColor);
    int getMovRestante();
    int getDir();
    void reducirMov();
    int getId();
    bool getActivo();
    QColor getColor();
    void pause();
    bool getRastro();
private:
    int                             tiempoMov_;
    int                             movimientoRestante_;
    int                             id_;
    int                             x_;
    int                             y_;
    std::thread                     hilo_;
    QWidget*                        padre_;
    QColor                          color_;
    int                             dir_;
    QGraphicsPixmapItem*            gPix_;
    bool                            activo_;
    bool                            rastro_;
public slots:
    void setRastro(bool);
};

#endif // AGENTE_H
