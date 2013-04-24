#pragma once

#include "Vector.h"

template <int dim, class T>
class Ray {
public:
  Vector<dim, T> start;
  Vector<dim, T> direction;
};
