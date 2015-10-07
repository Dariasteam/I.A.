#ifndef LABEL_H
#define LABEL_H
#include <QLabel>
#include <QGroupBox>

class celda : public QLabel{

    Q_OBJECT

public:
    celda(int,int);                         //posición i j
    void resizeEvent(QResizeEvent* );
private slots:
    void mouseMoveEvent(QMouseEvent* );
private:
    int i;
    int j;
    QPixmap* pix;
};

#endif // LABEL_H
