#ifndef LABEL_H
#define LABEL_H
#include <QLabel>
#include <QGroupBox>
#include "robot.h"

class celda : public QLabel{

    Q_OBJECT

public:
    celda(int,int,QPixmap*,QPixmap*,bool,QWidget* = 0);                         //posición i j
    ~celda();
    void resizeEvent(QResizeEvent* );
    void cambiarTipo(bool);
    bool atravesable();


private:
    bool atravesable_;
    int i_;
    int j_;
    QPixmap* pixSuelo_;
    QPixmap* pixMuro_;


};

#endif // LABEL_H
