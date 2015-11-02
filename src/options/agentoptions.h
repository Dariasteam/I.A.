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
};

#endif // AGENTOPTIONS_H
