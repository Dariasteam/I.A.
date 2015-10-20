#include "ficha.h"
#include "mainwindow.h"

#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>
#include <QDrag>
#include <QMimeData>

class MainWindow;

ficha::ficha(QString texto, QWidget* parent) : QGroupBox(){
    parent_ = parent;
    lay_ = new QBoxLayout(QBoxLayout::LeftToRight,this);
    lay_->setSizeConstraint(QLayout::SetFixedSize);
    labelBot_;
    labelBot_.setPixmap(QPixmap("../I.A./recursos/robotAbajo.png"));
    labelColor_;
    QPixmap P("../I.A./recursos/testigo.png");
    color_ = QColor(rand()%255+1,rand()%255+1,rand()%255+1);
    P.fill(color_);
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
            ((MainWindow*)parent_)->addAgente();
        }else if(isChecked()){
            QColorDialog* d = new QColorDialog(this);
            QPixmap P("../I.A./recursos/testigo.png");
            color_ = d->getColor();
            P.fill(color_);
            labelColor_.setPixmap(P);
        }
    }
}

void ficha::mousePressEvent(QMouseEvent* E){
    if(E->button()==Qt::LeftButton && this->geometry().contains(E->pos()) && this->isChecked()){
        QDrag* drag = new QDrag(this);
        QMimeData* mime = new QMimeData;
        mime->setText("mimeBot");
        drag->setMimeData(mime);
        drag->setPixmap(*(labelBot_.pixmap()));
        Qt::DropAction dropAction = drag->exec();
    }
}

QColor ficha::getColor(){
    return color_;
}
