#pragma once

#include "Vector.h"

template <class T>
class Quaternion
{
public:
    Quaternion ();
    Quaternion (T w, T v0, T v1, T v2);
    static Quaternion<T> angleAxis (T angle, Vector<3, T> const & axis);
	T & w ();
	T const & w () const;
	Vector<3, T> & v ();
	Vector<3, T> const & v () const;
    Quaternion<T> operator * (Quaternion<T> const & q) const;
    Quaternion<T> conjugate () const;
    Vector<3, T> rotate (Vector<3, T> const & v);

private:
	T w_;
	Vector<3, T> v_;
};

template <class T>
Quaternion<T>::Quaternion ()
: w (1), v ()
{
}

template <class T>
Quaternion<T>::Quaternion (T w, T v0, T v1, T v2)
: w_ (w), v_ (v0, v1, v2)
{
}

template <class T>
Quaternion<T>::angleAxis (T angle, Vector<3, T> const & axis)
{
    w_ = cos(angle / (T)2);
    v_ = axis * sin(angle / (T)2.0));
}

template <class T>
T & Quaternion<T>::w ()
{
	return w_;
}

template <class T>
T const & Quaternion<T>::w () const
{
	return w_;
}

template <class T>
Vector<3,T> & Quaternion<T>::v ()
{
	return v_;
}

template <class T>
Vector<3,T> const & Quaternion<T>::v () const
{
	return v_;
}

template <class T>
Quaternion<T> Quaternion<T>::operator * (Quaternion<T> const & q)
{
    Quaternion<T> r;
    r.w_ = w_ * q.w_ - v_[0] * q.v_[0] - v_[1] * q.v_[1] - v_[2] * q.v_[2];
	r.v_[0] = w_ * q.v_[0] + v_[0] * q.w_ + v_[1] * q.v_[2] - v_[2] * q.v_[1];
	r.v_[1] = w_ * q.v_[1] - v_[0] * q.v_[2] + v_[1] * q.w_ + v_[2] * q.v_[0];
	r.v_[2] = w_ * q.v_[2] + v_[0] * q.v_[1] - v_[1] * q.v_[0] + v_[2] * q.w_;
    return r;
}

template <class T>
Quaternion<T> Quaternion<T>::conjugate () const
{
    Quaternion<T> r;
    r.w_ = w_;
    r.v_ = v_ * -1;
    return r;
}

template <class T>
Vector<3, T> Quaternion<T>::rotate (Vector<3, T> const & v)
{
    return ((*this) * Quaternion<T>(0, v[0], v[1], v[2]) * conjugate()).v();
}

