#include "agent.hpp"

Agent::Agent(int x, int y, Map & map):
  x_(x),
  y_(y),
  sensors_(Sensors(&map, &x, &y)) {}


Agent::~Agent() {}


Sensors & Agent::getSensors(void) {
  return sensors_;
}


Output Agent::operator()(void) {
  // TODO: Aqui viene a aplicarse toda la IA
  // TODO: Ver como representaremos la memoria
}
