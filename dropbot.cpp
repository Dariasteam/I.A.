#include "dropbot.h"

dropbot::dropbot(QWidget* parent) : QLabel(parent){

}

void dropbot::mousePressEvent(QMouseEvent* E){
    if(E->button()==Qt::LeftButton && this->geometry().contains(E->pos())){
        QDrag* drag = new QDrag(this);
        QMimeData* mime = new QMimeData;
        mime->setText("mimeBot");
        drag->setMimeData(mime);
        drag->setPixmap(*this->pixmap());
        Qt::DropAction dropAction = drag->exec();
    }
}
