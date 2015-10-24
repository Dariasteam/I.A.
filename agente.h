#ifndef agente_H
#define agente_H

//#include "agente.h"

#include <QGroupBox>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QColorDialog>
#include <QColor>
#include <QCheckBox>
#include <thread>
#include <QGraphicsPixmapItem>


class agente : public QGroupBox{
public:
    agente(QString texto, int x, int y, int tiempoMov_, int id, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    QColor getColor();
    void desactivarSegir();
    int getId();
    void movimiento();
    void start();
    void finMovimiento();
    int getMovRestante();
    int getDir();
    void reducirMov();
    bool getActivo();
    void pause();
    bool getRastro();
    bool getSeguir();
    void setSeguir(bool);
private:
    int                             tiempoMov_;
    int                             movimientoRestante_;
    int                             id_;
    int                             x_;
    int                             y_;
    std::thread                     hilo_;
    int                             dir_;
    QGraphicsPixmapItem*            gPix_;
    bool                            rastro_;
    bool                            activo_;
    bool                            seguir_;
    QLabel                          labelBot_;
    QLabel                          labelText_;
    QLabel                          labelColor_;
    QGridLayout*                    lay_;
    QWidget*                        parent_;
    QColor                          color_;
    QCheckBox*                      checkRastro_;
    QCheckBox*                      checkSeguir_;
signals:

public slots:
    void check(bool);
    void checkSeguir(bool);
};



/*class agente : public QWidget{


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
    bool getSeguir();
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
    bool                            seguir_;
public slots:
    void setRastro(bool);
    void setSeguir(bool);
};
*/

#endif // agente_H
