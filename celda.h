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
    bool atravesable();
private:
    QPixmap* pixSuelo_;
    QPixmap* pixMuro_;
    bool atravesable_;
};

#endif // LABEL_H
