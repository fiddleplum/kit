#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <exception>
#include <cmath>

template <typename T>
class Quaternion
{
public:
	Quaternion();

	Quaternion(T r, T i, T j, T k);

	Quaternion(T r, Vector<3, T> ijk);

	Quaternion(T angle, Vector<3, T> const & axis, bool axisIsNormalized);

	Quaternion(Vector<3, T> const & startVector, Vector<3, T> const & endVector, bool vectorsAreNormalized);

	Quaternion(T yaw, T pitch, T roll);

	Quaternion<T> conjugate() const;

	Quaternion<T> inverse() const; // alias for conjugate

	Quaternion<T> reciprocal() const;

	T norm() const;

	T normSq() const;

	void normalize();

	Vector<3, T> rotate(Vector<3, T> const & v) const; // assumes this is normalized

	Vector<3, T> getAxis(unsigned int i) const; // assumes this is normalized

	Matrix<3, 3, T> getMatrix() const; // for pre multiplying, assumes this is normalized

	T r;

	Vector<3, T> ijk;
};

typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;

template <typename T>
Quaternion<T> operator +(Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs);

template <typename T>
Quaternion<T> operator -(Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs);

template <typename T>
Quaternion<T> operator *(Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs);

template <typename T>
Quaternion<T> operator *(Quaternion<T> const & q, T t);

template <typename T>
Quaternion<T> operator *(T t, Quaternion<T> const & q);

// Template implementation

template <typename T>
Quaternion<T>::Quaternion()
: r(1), ijk(0, 0, 0)
{
}

template <typename T>
Quaternion<T>::Quaternion(T r, T i, T j, T k)
: r(r), ijk(i, j, k)
{
}

template <typename T>
Quaternion<T>::Quaternion(T r, Vector<3, T> ijk)
: r(r), ijk(ijk)
{
}

template <typename T>
Quaternion<T>::Quaternion(Vector<3, T> const & startVector, Vector<3, T> const & endVector, bool vectorsAreNormalized)
{
	r = startVector.dot(endVector);
	Vector<3, T> axis;
	if(r != -1)
	{
		ijk = startVector.cross(endVector);
	}
	else
	{
		ijk = startVector.perpendicular();
	}
	if(vectorsAreNormalized)
	{
		r += 1;
	}
	else
	{
		r += startVector.norm() * endVector.norm();
	}
	normalize();
}

template <typename T>
Quaternion<T>::Quaternion(T angle, Vector<3, T> const & axis, bool axisIsNormalized)
{
	r = std::cos(angle / (T)2);
	if(axisIsNormalized)
	{
		ijk = axis;
	}
	else
	{
		ijk = axis.unit();
	}
	ijk *= std::sin(angle / (T)2);
}

template <typename T>
Quaternion<T>::Quaternion(T yaw, T pitch, T roll)
{
	T cosYaw2 = cos(yaw / 2);
	T sinYaw2 = sin(yaw / 2);
	T cosPitch2 = cos(pitch / 2);
	T sinPitch2 = sin(pitch / 2);
	T cosRoll2 = cos(roll / 2);
	T sinRoll2 = sin(roll / 2);
	r =      cosYaw2 * cosPitch2 * cosRoll2 + sinYaw2 * sinPitch2 * sinRoll2;
	ijk[1] = cosYaw2 * cosPitch2 * sinRoll2 - sinYaw2 * sinPitch2 * cosRoll2;
	ijk[0] = -cosYaw2 * sinPitch2 * cosRoll2 - sinYaw2 * cosPitch2 * sinRoll2;
	ijk[2] = sinYaw2 * cosPitch2 * cosRoll2 - cosYaw2 * sinPitch2 * sinRoll2;
}

template <typename T>
Quaternion<T> Quaternion<T>::conjugate() const
{
	return Quaternion<T>(r, -ijk);
}

template <typename T>
Quaternion<T> Quaternion<T>::inverse() const
{
	return conjugate();
}

template <typename T>
Quaternion<T> Quaternion<T>::reciprocal() const
{
	T nSq = normSq();
	if(nSq == 0)
	{
		throw std::exception();
	}
	return conjugate() / nSq;
}

template <typename T>
T Quaternion<T>::norm() const
{
	return std::sqrt((r * r) + ijk.normSq());
}

template <typename T>
T Quaternion<T>::normSq() const
{
	return (r * r) + ijk.normSq();
}

template <typename T>
void Quaternion<T>::normalize()
{
	T n = norm();
	if(n == 0)
	{
		throw std::exception();
	}
	r /= n;
	ijk /= n;
}

template <typename T>
Vector<3, T> Quaternion<T>::rotate(Vector<3, T> const & v) const
{
	Vector<3, T> t = (T)2 * ijk.cross(v);
	return v + r * t + ijk.cross(t);
}

template <typename T>
Vector<3, T> Quaternion<T>::getAxis(unsigned int i) const
{
	if(i >= 3)
	{
		throw std::exception();
	}
	Vector3f axis;
	unsigned int j = (i + 1) % 3;
	unsigned int k = (i + 2) % 3;
	axis[i] = 1.0f - 2.0f * (ijk[j] * ijk[j] + ijk[k] * ijk[k]);
	axis[j] = 2.0f * (ijk[i] * ijk[j] - ijk[k] * r);
	axis[k] = 2.0f * (ijk[i] * ijk[k] + ijk[j] * r);
	return axis;
}

template <typename T>
Matrix<3, 3, T> Quaternion<T>::getMatrix() const
{
	Matrix<3, 3, T> m;
	float ii = ijk[0] * ijk[0];
	float ij = ijk[0] * ijk[1];
	float ik = ijk[0] * ijk[2];
	float ir = ijk[0] * r;
	float jj = ijk[1] * ijk[1];
	float jk = ijk[1] * ijk[2];
	float jr = ijk[1] * r;
	float kk = ijk[2] * ijk[2];
	float kr = ijk[2] * r;
	m(0, 0) = (T)1 - (T)2 * (jj + kk);
	m(1, 0) = (T)2 * (ij - kr);
	m(2, 0) = (T)2 * (ik + jr);
	m(0, 1) = (T)2 * (ij + kr);
	m(1, 1) = (T)1 - (T)2 * (ii + kk);
	m(2, 1) = (T)2 * (jk - ir);
	m(0, 2) = (T)2 * (ik - jr);
	m(1, 2) = (T)2 * (jk + ir);
	m(2, 2) = (T)1 - (T)2 * (ii + jj);
	return m;
}

template <typename T>
Quaternion<T> operator +(Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs)
{
	return Quaternion<T>(q_lhs.r + q_rhs.r, q_lhs.ijk + q_rhs.ijk);
}

template <typename T>
Quaternion<T> operator -(Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs)
{
	return Quaternion<T>(q_lhs.r - q_rhs.r, q_lhs.ijk - q_rhs.ijk);
}

template <typename T>
Quaternion<T> operator *(Quaternion<T> const & q_lhs, Quaternion<T> const & q_rhs)
{
	return Quaternion<T>(q_lhs.r * q_rhs.r - q_lhs.ijk.dot(q_rhs.ijk), q_lhs.r * q_rhs.ijk + q_rhs.r * q_lhs.jik + q_lhs.ijk.cross(q_rhs.ijk));
}

template <typename T>
Quaternion<T> operator *(Quaternion<T> const & q, T t)
{
	return Quaternion<T>(q.r * t, q.ijk * t);
}

template <typename T>
Quaternion<T> operator *(T t, Quaternion<T> const & q)
{
	return Quaternion<T>(t * q.r, t * q.ijk);
}

