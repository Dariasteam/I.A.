#ifndef DRAGAGENT_H
#define DRAGAGENT_H

#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>


class DragAgent : public QLabel
{
    Q_OBJECT
public:
    explicit DragAgent(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent* );

signals:

public slots:
};

#endif // DRAGAGENT_H
