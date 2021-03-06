#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <string>

class Vector3D {
public:
  int x;
  int y;
  int z;

  Vector3D();
  Vector3D(int _x, int _y, int _z);
  Vector3D(const Vector3D &v);
  ~Vector3D();
  
  std::string getString() const;
  
};

Vector3D operator+(const Vector3D  &v1, const Vector3D &v2);
Vector3D operator/(const Vector3D &v1, const int &i);
bool operator==(const Vector3D &v1, const Vector3D &v2);
bool operator!=(const Vector3D &v1, const Vector3D &v2);

#endif
