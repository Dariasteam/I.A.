#ifndef FICHA_H
#define FICHA_H

#include <QGroupBox>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QBoxLayout>
#include <QString>
#include <QColorDialog>
#include <QColor>
#include "agente.h"

class agente;

class ficha : public QGroupBox{
public:
    ficha(QString texto, agente*, QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* );
    QColor getColor();
private:
    QLabel          labelBot_;
    QLabel          labelText_;
    QLabel          labelColor_;
    QBoxLayout*     lay_;
    bool            activo_;
    QWidget*        parent_;
    QColor          color_;
    agente*         agente_;
signals:

public slots:
    void check(bool);
};

#endif // FICHA_H
