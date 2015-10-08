#ifndef LABEL_H
#define LABEL_H
#include <QLabel>
#include <QGroupBox>

class celda : public QLabel{

    Q_OBJECT

public:
    celda(int,int,QPixmap*,QPixmap*,bool,QWidget* = 0);                         //posición i j
    ~celda();
    void resizeEvent(QResizeEvent* );
    void cambiarTipo(bool);
private:
    bool atravesable_;
    int i_;
    int j_;
    QPixmap* pixSuelo_;
    QPixmap* pixMuro_;

};

#endif // LABEL_H
