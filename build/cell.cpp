#include "cell.hpp"

Cell::Cell() {}

Cell::Cell(Vector2D &p) {
  position = p;
}

Cell::Cell(Vector2D &p, Vector3D &c) {
  position = p;
  color = c;
}

Cell::Cell(const Cell &n) {
  position = n.position;
  color = n.color;
}

Cell::~Cell() {}
