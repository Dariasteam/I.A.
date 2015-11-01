#pragma once

#include <QList>
#include "../map.h"


using namespace std;
struct CellWeight;

struct Pos {
    int x_;
    int y_;
};

struct Path {
    QList<Pos> path_;
    double cost_;
};

class Agent {
public:
  int x_;
  int y_;
  bool noFinded_;
  Path * possibleEnd_;
  QList<Path> paths_;
  Path joinPath_;
public:
  Agent(int,int);
  ~Agent();
  Pos operator()(QList<CellWeight>);
  void insertSort(Path path);
  void pathJoin(Path p, Path p2);
};
