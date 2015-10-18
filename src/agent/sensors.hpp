#pragma once
#include "../map/map.hpp"

class Sensors {
private:
  // | Un acceso al mapa
  Map * map_;
  // | Acceso a la posicion del agente
  int * x_;
  int * y_;
public:
  enum Info {
    Block,
    Free
  };
  // | Solo necesito donde estoy situado dentro del mapa
  Sensors(Map *, int *, int *);
  Info getInfoFromCell(Cell);
  // | No sirve de nada, quizas si estamos en hilo independiente para ver las actualizaciones de pos
  // void detectEnv();
  // | What are there?
  Info left(void);
  // | What are there?
  Info right(void);
  // | What are there?
  Info up(void);
  // | What are there?
  Info down(void);
};
