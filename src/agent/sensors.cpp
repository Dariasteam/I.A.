#include "sensors.hpp"
#include "../cell/cell.hpp"

Sensors::Sensors(Map * map, int * x, int *y):
  map_(map),
  x_(x),
  y_(y) {}


Sensors::Info Sensors::getInfoFromCell(Cell cell) {
  return (cell.walkable() ? Free:Block);
}


Sensors::Info Sensors::left(void) {
  return getInfoFromCell((*map_)(*x_,*y_-1));
}


Sensors::Info Sensors::right(void) {
  return getInfoFromCell((*map_)(*x_,*y_+1));
}


Sensors::Info Sensors::up(void) {
  return getInfoFromCell((*map_)(*x_+1,*y_));
}


Sensors::Info Sensors::down(void) {
  return getInfoFromCell((*map_)(*x_-1,*y_));
}
