#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

template <typename T>
class Frame
{
public:
	Matrix<4, 4, T> getMatrix() const;

	Matrix<4, 4, T> getMatrixInverse() const;

	Vector<3, T> pos;

	Quaternion<T> ori;

	Vector<3, T> scale;
};

// Template implementation

template <typename T>
Matrix<4, 4, T> Frame<T>::getMatrix() const
{
	Matrix<4, 4, T> m;
	Matrix<3, 3, T> rot = ori.getMatrix();
	m(0, 0) = rot(0, 0) * scale[0];
	m(1, 0) = rot(1, 0) * scale[0];
	m(2, 0) = rot(2, 0) * scale[0];
	m(3, 0) = 0;
	m(0, 1) = rot(0, 1) * scale[1];
	m(1, 1) = rot(1, 1) * scale[1];
	m(2, 1) = rot(2, 1) * scale[1];
	m(3, 1) = 0;
	m(0, 2) = rot(0, 2) * scale[2];
	m(1, 2) = rot(1, 2) * scale[2];
	m(2, 2) = rot(2, 2) * scale[2];
	m(3, 2) = 0;
	m(0, 3) = pos[0];
	m(1, 3) = pos[1];
	m(2, 3) = pos[2];
	m(3, 3) = 1;
	return m;
}

template <typename T>
Matrix<4, 4, T> Frame<T>::getMatrixInverse() const
{
	if(scale[0] == 0 || scale[1] == 0 || scale[2] == 0)
	{
		throw std::exception();
	}
	Matrix<4, 4, T> m;
	Matrix<3, 3, T> rot = ori.getMatrix();
	m(0, 0) = rot(0, 0) / scale[0];
	m(1, 0) = rot(0, 1) / scale[1];
	m(2, 0) = rot(0, 2) / scale[2];
	m(3, 0) = 0;
	m(0, 1) = rot(1, 0) / scale[0];
	m(1, 1) = rot(1, 1) / scale[1];
	m(2, 1) = rot(1, 2) / scale[2];
	m(3, 1) = 0;
	m(0, 2) = rot(2, 0) / scale[0];
	m(1, 2) = rot(2, 1) / scale[1];
	m(2, 2) = rot(2, 2) / scale[2];
	m(3, 2) = 0;
	m(0, 3) = (-pos[0] * rot(0, 0) - pos[1] * rot(1, 0) - pos[2] * rot(2, 0)) / scale[0];
	m(1, 3) = (-pos[0] * rot(0, 1) - pos[1] * rot(1, 1) - pos[2] * rot(2, 1)) / scale[1];
	m(2, 3) = (-pos[0] * rot(0, 2) - pos[1] * rot(1, 2) - pos[2] * rot(2, 2)) / scale[2];
	m(3, 3) = 1;
	return m;
}

