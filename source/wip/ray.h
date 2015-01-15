#pragma once

#include "coord.h"

template <unsigned int dim, typename T>
class Ray
{
public:
	Ray();
	Ray(Coord<dim, T> start, Coord<dim, T> direction);

	Ray<dim, T> scaled(float factor);  // only affects direction

	Coord<dim, T> start;
	Coord<dim, T> direction;
};

typedef Ray<2, int> Ray2i;
typedef Ray<2, float> Ray2f;
typedef Ray<2, double> Ray2d;
typedef Ray<3, int> Ray3i;
typedef Ray<3, float> Ray3f;
typedef Ray<3, double> Ray3d;

template <unsigned int dim, typename T>
Ray<dim, T>::Ray()
{
}

template <unsigned int dim, typename T>
Ray<dim, T>::Ray(Coord<dim, T> newStart, Coord<dim, T> newDirection)
{
	start = newStart;
	direction = newDirection;
}

template <unsigned int dim, typename T>
Ray<dim, T> Ray<dim, T>::scaled(float factor)
{
	Ray<dim, T> r;
	r.start = start;
	r.direction = direction * factor;
	return r;
}

