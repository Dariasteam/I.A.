#ifndef AGENTTOKEN_H
#define AGENTTOKEN_H

#include <QWidget>
#include <QColorDialog>
#include "../map.h"

namespace Ui {
class AgentToken;
}

class AgentToken : public QWidget
{
    Q_OBJECT

public:
    explicit AgentToken(Map* map, int id, QWidget *parent = 0);
    ~AgentToken();

private:
    int idAgent_;
    QColor color_;
    Ui::AgentToken *ui;

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // AGENTTOKEN_H
