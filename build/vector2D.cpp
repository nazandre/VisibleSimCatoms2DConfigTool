#include "vector2D.hpp"

using namespace std;

Vector2D::Vector2D() {};

Vector2D::Vector2D(int _x, int _y) {
  x = _x;
  y = _y;
}

Vector2D::Vector2D(const Vector2D &v) {
  x = v.x;
  y = v.y;
}

Vector2D::~Vector2D() {}

Vector2D operator+(const Vector2D  &v1, const Vector2D &v2) {
  return Vector2D(v1.x+v2.x,v1.y+v2.y);
}

Vector2D operator/(const Vector2D  &v1, const int &i) {
  return Vector2D(v1.x/i,v1.y/i);
}

string Vector2D::getString3D(int v) const {
  return to_string(x) + "," + to_string(v) + "," + to_string(y);
}
