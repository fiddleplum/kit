#pragma once

#include "vector.h"

template <typename T>
class Circle
{
public:
	Vector<2, T> center;
	T radius;
};

typedef Circle<float> Circle2f;
typedef Circle<double> Circle2d;

