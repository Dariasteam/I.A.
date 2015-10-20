#include "ficha.h"
#include "mapa.h"

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>

ficha::ficha(QString texto, QWidget* parent) : QGroupBox(parent){
    lay_ = new QBoxLayout(QBoxLayout::LeftToRight,this);
    lay_->setSizeConstraint(QLayout::SetFixedSize);
    labelBot_;
    labelBot_.setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));
    labelColor_;
    QPixmap P("../I.A./recursos/testigo.png");
    P.fill(QColor(40,40,40,255));
    labelColor_.setPixmap(P);
    lay_->addWidget(&labelBot_);
    labelText_.setText(texto);
    lay_->addWidget(&labelText_);
    lay_->addWidget(&labelColor_);
    activo_ = false;
    labelBot_.setEnabled(false);
    labelColor_.setEnabled(false);
    labelText_.setEnabled(false);
}

void ficha::mouseDoubleClickEvent(QMouseEvent* E){
    if(E->button() ==Qt::LeftButton){
        if(!activo_){
            labelBot_.setEnabled(true);
            labelColor_.setEnabled(true);
            labelText_.setEnabled(true);
            setCheckable(true);
            activo_=true;
            ((mapa*)parent());
        }else if(isChecked()){
            QColorDialog* d = new QColorDialog(this);
            QPixmap P("../I.A./recursos/testigo.png");
            P.fill(d->getColor());
            labelColor_.setPixmap(P);
        }
    }
}

void ficha::keyPressEvent(QKeyEvent* E){
    printf("WWWW");
    if(E->key()==Qt::Key_Delete){
        printf("dsadasdasd");
    }
}

