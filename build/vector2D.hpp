#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <string>

class Vector2D {
public:
  int x;
  int y;

  Vector2D();
  Vector2D(int _x, int _y);
  Vector2D(const Vector2D &v);
  ~Vector2D();

  std::string getString3D(int v) const;
};

Vector2D operator+(const Vector2D  &v1, const Vector2D &v2);
Vector2D operator/(const Vector2D &v1, const int &i);

#endif
