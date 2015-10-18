#pragma once

#include "../map.h"
#include "output.hpp"
#include "sensors.hpp"

namespace Model {

class Agent {
private:
  int x_;
  int y_;
  Sensors sensors_;
public:
  Agent(int,int,Map &);
  ~Agent();
  // | Get sensors aviable of agent, useful on a interaction clock' ticks.
  Sensors & getSensors(void);
  Output operator()(void);
};
}
