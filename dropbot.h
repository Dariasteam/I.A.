#ifndef DROPBOT_H
#define DROPBOT_H

#include <QWidget>
#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

class dropbot : public QLabel{
public:
    dropbot(QWidget* parent);
    void mousePressEvent(QMouseEvent* );
};

#endif // DROPBOT_H
