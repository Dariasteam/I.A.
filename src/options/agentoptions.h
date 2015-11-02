#ifndef AGENTOPTIONS_H
#define AGENTOPTIONS_H

#include <QWidget>
#include "dragagent.h"
#include "agenttoken.h"
#include "../agent/agent.hpp"
#include "../map/map.h"

namespace Ui {
class AgentOptions;
}

class AgentOptions : public QWidget
{
    Q_OBJECT

public:
    explicit AgentOptions(QWidget *parent = 0);
    ~AgentOptions();

private:
    Ui::AgentOptions *ui;
public slots:
    void createAgentInfo(Map* map, int id);

signals:
    void simulate(bool stop);
    void step(void);
    void speed(int);
};

#endif // AGENTOPTIONS_H
