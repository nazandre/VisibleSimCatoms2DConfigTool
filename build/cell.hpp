#ifndef CELL_H_
#define CELL_H_

#include "vector2D.hpp"
#include "vector3D.hpp"

class Cell {
public:
  Vector3D color;
  Vector2D position;
  int id;

  Cell();
  Cell(Vector2D &p);
  Cell(Vector2D &p, Vector3D &c);
  Cell(const Cell &n);
  ~Cell();
};

#endif
