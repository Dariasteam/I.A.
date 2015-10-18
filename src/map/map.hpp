#pragma once
#include <vector>
#include <string>
#include "../cell/cell.hpp"

using namespace std;

class Map {
private:
  vector<Cell> map_;
  int height_;
  int width_;
public:
  Map(Cell, int, int);
  Map(string);
  virtual ~Map();
  Cell & operator()(const int, const int);
  Cell operator()(const int, const int) const;
  void readMapFromFile(string);
  void writeMapInFile(string);
};
