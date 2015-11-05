#ifndef DROPBOT_H
#define DROPBOT_H

#include <QWidget>
#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

class dropbot : public QLabel{
public:
    dropbot(short id, QWidget* parent);
    void mousePressEvent(QMouseEvent* );
private:
    short id_;
};

#endif // DROPBOT_H
