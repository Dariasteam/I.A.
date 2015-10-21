#include "ficha.h"
#include "mainwindow.h"

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>


class MainWindow;

ficha::ficha(QString texto, agente* a, QWidget* parent) : QGroupBox(){
    parent_ = parent;
    agente_=a;
    lay_ = new QBoxLayout(QBoxLayout::LeftToRight,this);
    lay_->setSizeConstraint(QLayout::SetFixedSize);
    labelBot_.setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));
    QPixmap P("../I.A./recursos/testigo.png");
    color_ = QColor(rand()%255+1,rand()%255+1,rand()%255+1);
    P.fill(color_);
    labelColor_.setPixmap(P);
    lay_->addWidget(&labelBot_);
    labelText_.setText(texto);
    lay_->addWidget(&labelText_);
    lay_->addWidget(&labelColor_);
    agente_->setColor(color_);
    setCheckable(true);
    connect(this,&QGroupBox::clicked,this,&ficha::check);
}

void ficha::mouseDoubleClickEvent(QMouseEvent* E){
    if(E->button() ==Qt::LeftButton){
        if(isChecked()){
            QColorDialog* d = new QColorDialog(this);
            QPixmap P("../I.A./recursos/testigo.png");
            color_ = d->getColor();
            P.fill(color_);
            labelColor_.setPixmap(P);
            agente_->setColor(color_);
        }
    }
}

QColor ficha::getColor(){
    return color_;
}

void ficha::check(bool b){
    if(b){
        agente_->start();
    }else{
        agente_->pause();
    }
}
