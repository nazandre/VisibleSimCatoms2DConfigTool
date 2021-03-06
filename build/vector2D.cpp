#include "vector2D.hpp"

using namespace std;

Vector2D::Vector2D() {
  x = 0;
  y = 0;
};

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

bool operator==(const Vector2D &v1, const Vector2D &v2) {
  return (v1.x == v2.x) && (v1.y == v2.y);
}

bool operator!=(const Vector2D &v1, const Vector2D &v2) {
  return (v1.x != v2.x) || (v1.y != v2.y);
}

string Vector2D::getString3D(int v) const {
  return to_string(x) + "," + to_string(v) + "," + to_string(y);
}

string Vector2D::getString2D() const {
  return to_string(x) + "," + to_string(y);
}

