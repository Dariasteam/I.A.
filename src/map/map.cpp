#include "map.hpp"
#include <fstream>

Map::Map(Cell data, int width, int height):
  map_(width*height),
  height_(height),
  width_(width)
{
  for (int i=0; i < width_*height_; i++) {
    map_[i]=data;
  }
}


Map::Map(string filepath) {
  readMapFromFile(filepath);
}


Map::~Map(void) {}


Cell & Map::operator()(const int i, const int j){
  return (map_[i+j*width_]);
}


Cell Map::operator()(const int i, const int j) const {
  return (map_[i+j*width_]);
}


void Map::readMapFromFile(string filepath){
  ifstream file;
  file.open(filepath.c_str());
  if (file.is_open()) {
    file >> width_;
    file >> height_;
    while (!file.eof()) {
      Cell aux;
      // file >> aux; // TODO this not it is definitive don't work
      map_.push_back(aux);
    }
  }
}


void Map::writeMapInFile(string filepath){
  ofstream file;
  file.open(filepath.c_str());
  if (file.is_open()) {
    file << width_ << endl;
    file << height_ << endl;
    for (int i=0; i < map_.size(); i++) {}
      // file << map_[i] << " ";
  }
  else {
    // TODO throw an error
  }
}
