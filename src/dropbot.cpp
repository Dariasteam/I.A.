#include "dropbot.h"
#include <iostream>

dropbot::dropbot(short id, QWidget* parent) : QLabel(parent){
    id_=id;
}

void dropbot::mousePressEvent(QMouseEvent* E){
    if(E->button()==Qt::LeftButton){
        QDrag* drag = new QDrag(this);
        QMimeData* mime = new QMimeData;
        mime->setText(QString::fromStdString(std::to_string(id_)));
        drag->setMimeData(mime);
        drag->setPixmap(*this->pixmap());
        Qt::DropAction dropAction = drag->exec();
    }
}
