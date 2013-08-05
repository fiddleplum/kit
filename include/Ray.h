#pragma once

#include "Vector.h"

template <int dim, class T>
class Ray
{
public:
	Vector<dim, T> start;
	Vector<dim, T> direction;
};

typedef Ray<2, int> Ray2i;
typedef Ray<2, float> Ray2f;
typedef Ray<2, double> Ray2d;
typedef Ray<3, int> Ray3i;
typedef Ray<3, float> Ray3f;
typedef Ray<3, double> Ray3d;

