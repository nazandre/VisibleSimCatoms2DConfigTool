#include "vector3D.hpp"

using namespace std;

Vector3D::Vector3D() {};

Vector3D::Vector3D(int _x, int _y, int _z) {
  x = _x;
  y = _y;
  z = _z;
}

Vector3D::Vector3D(const Vector3D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3D::~Vector3D() {}

string Vector3D::getString() const {
  return to_string(x) + "," + to_string(y) + "," + to_string(z);
}

Vector3D operator+(const Vector3D  &v1, const Vector3D &v2) {
  return Vector3D(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}

Vector3D operator/(const Vector3D  &v1, const int &i) {
  return Vector3D(v1.x/i,v1.y/i,v1.z/i);
}

bool operator==(const Vector3D &v1, const Vector3D &v2) {
  return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

bool operator!=(const Vector3D &v1, const Vector3D &v2) {
  return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
}

