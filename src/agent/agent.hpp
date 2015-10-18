#pragma once

#include "../map/map.hpp"
#include "output.hpp"
#include "sensors.hpp"

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
