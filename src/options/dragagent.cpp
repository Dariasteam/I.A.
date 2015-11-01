#include "dragagent.h"

DragAgent::DragAgent(QWidget *parent) : QLabel(parent) {
    setPixmap(QPixmap(":/recursos/robotAbajo.png"));
}

void DragAgent::mousePressEvent(QMouseEvent * ev) {
    if (ev->button()==Qt::LeftButton && geometry().contains(ev->pos())) {
        QDrag* drag = new QDrag(this);
        QMimeData * mime = new QMimeData;
        mime->setText("mimeBot");
        drag->setMimeData(mime);
        drag->setPixmap(*pixmap());
        drag->exec();
    }
}
