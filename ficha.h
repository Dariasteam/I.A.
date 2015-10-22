#ifndef FICHA_H
#define FICHA_H

#include "agente.h"

#include <QGroupBox>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QColorDialog>
#include <QColor>
#include <QCheckBox>


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
    QGridLayout*    lay_;
    bool            activo_;
    QWidget*        parent_;
    QColor          color_;
    agente*         agente_;
    QCheckBox*      checkRastro_;
    //QCheckBox*      checkMem_;
signals:

public slots:
    void check(bool);
};

#endif // FICHA_H
