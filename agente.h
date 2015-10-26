#ifndef agente_H
#define agente_H

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
    agente(QString texto, int x, int y, double tiempoMov_, int id,QGraphicsPixmapItem* gPix, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    QColor getColor();
    void detontante();
    void desactivarSegir();
    void movimiento();
    void start();
    void finMovimiento();
    bool getActivo();
    void pause();
    void setRastro(bool);
    void unselectSeguir();
    int getX();
    int getY();
private:
    int                             tiempoMov_;
    int                             movimientoRestante_;
    int                             id_;
    int                             x_;
    int                             y_;
    std::thread                     hilo_;
    int                             dir_;
    QGraphicsPixmapItem*            gPix_;
    bool                            activo_;
    QLabel                          labelBot_;
    QLabel                          labelText_;
    QLabel                          labelColor_;
    QGridLayout*                    lay_;
    QWidget*                        parent_;
    QColor                          color_;
    QCheckBox*                      checkRastro_;
    QCheckBox*                      checkSeguir_;
    QTimer*                         tiempo_;
public slots:
    void check(bool);
    void checkSeguir();
    void animador();
};

#endif // agente_H
