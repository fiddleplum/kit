#pragma once

#include "../Vector.h"
#include "../Quaternion.h"
#include "../Matrix.h"

template <typename T>
class Frame
{
public:
	Frame();
	Vector<3, T> const & getPosition() const;
	Vector<3, T> & getPosition();
	Quaternion<T> const & getOrientation() const;
	Quaternion<T> & getOrientation();
	Vector<3, T> const & getScale() const;
	Vector<3, T> & getScale();
	Matrix<4, 4, T> const & getMatrix() const;
	Matrix<4, 4, T>  const & getMatrixInverse() const;

private:
	void updateMatrices();

	Vector<3, T> mPosition;
	Quaternion<T> mOrientation;
	Vector<3, T> mScale;
	bool mMatricesNeedUpdate;
	Matrix<4, 4, T> mMatrix;
	Matrix<4, 4, T> mMatrixInverse;
};

typedef Frame<float> Framef;
typedef Frame<double> Framed;

// Template implementation

template <typename T>
Frame<T>::Frame()
{
	mPosition = Vector<3, T>::zero();
	mScale = Vector<3, T>::filled(1);
	mMatrixInverse = mMatrix = Matrix<4, 4, T>::identity();
	mMatricesNeedUpdate = false;
}

template <typename T>
Vector<3, T> const & Frame<T>::getPosition() const
{
	return mPosition;
}

template <typename T>
Vector<3, T> & Frame<T>::getPosition()
{
	mMatricesNeedUpdate = true;
	return mPosition;
}

template <typename T>
Quaternion<T> const & Frame<T>::getOrientation() const
{
	return mOrientation;
}

template <typename T>
Quaternion<T> & Frame<T>::getOrientation()
{
	mMatricesNeedUpdate = true;
	return mOrientation;
}

template <typename T>
Vector<3, T> const & Frame<T>::getScale() const
{
	return mScale;
}

template <typename T>
Vector<3, T> & Frame<T>::getScale()
{
	mMatricesNeedUpdate = true;
	return mScale;
}

template <typename T>
Matrix<4, 4, T> const & Frame<T>::getMatrix() const
{
	if(mMatricesNeedUpdate)
	{
		const_cast<Frame<T> *>(this)->updateMatrices();
	}
	return mMatrix;
}

template <typename T>
Matrix<4, 4, T>  const & Frame<T>::getMatrixInverse() const
{
	if(mMatricesNeedUpdate)
	{
		const_cast<Frame<T> *>(this)->updateMatrices();
	}
	return mMatrixInverse;
}

template <typename T>
void Frame<T>::updateMatrices()
{
	Matrix<3, 3, T> rot = mOrientation.getMatrix();
	mMatrix(0, 0) = rot(0, 0) * mScale[0];
	mMatrix(1, 0) = rot(1, 0) * mScale[0];
	mMatrix(2, 0) = rot(2, 0) * mScale[0];
	mMatrix(0, 1) = rot(0, 1) * mScale[1];
	mMatrix(1, 1) = rot(1, 1) * mScale[1];
	mMatrix(2, 1) = rot(2, 1) * mScale[1];
	mMatrix(0, 2) = rot(0, 2) * mScale[2];
	mMatrix(1, 2) = rot(1, 2) * mScale[2];
	mMatrix(2, 2) = rot(2, 2) * mScale[2];
	mMatrix(0, 3) = mPosition[0];
	mMatrix(1, 3) = mPosition[1];
	mMatrix(2, 3) = mPosition[2];
	mMatrixInverse(0, 0) = rot(0, 0) / mScale[0];
	mMatrixInverse(1, 0) = rot(0, 1) / mScale[1];
	mMatrixInverse(2, 0) = rot(0, 2) / mScale[2];
	mMatrixInverse(0, 1) = rot(1, 0) / mScale[0];
	mMatrixInverse(1, 1) = rot(1, 1) / mScale[1];
	mMatrixInverse(2, 1) = rot(1, 2) / mScale[2];
	mMatrixInverse(0, 2) = rot(2, 0) / mScale[0];
	mMatrixInverse(1, 2) = rot(2, 1) / mScale[1];
	mMatrixInverse(2, 2) = rot(2, 2) / mScale[2];
	mMatrixInverse(0, 3) = (-mPosition[0] * rot(0, 0) - mPosition[1] * rot(1, 0) - mPosition[2] * rot(2, 0)) / mScale[0];
	mMatrixInverse(1, 3) = (-mPosition[0] * rot(0, 1) - mPosition[1] * rot(1, 1) - mPosition[2] * rot(2, 1)) / mScale[1];
	mMatrixInverse(2, 3) = (-mPosition[0] * rot(0, 2) - mPosition[1] * rot(1, 2) - mPosition[2] * rot(2, 2)) / mScale[2];
	mMatricesNeedUpdate = false;
}


