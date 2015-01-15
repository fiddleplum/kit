#pragma once

#include <initializer_list>
#include <exception>
#include <cassert>
#include <cmath>

// This is a standard mathematical vector class. Dim is the dimensions of the vector and T is the type of its elements.
template <unsigned int dim, typename T>
class Coord
{
public:
	// Default constructor. Zeroes all elements.
	Coord();

	// Copy constructor. Each element in v is converted from type Y to type T.
	template <typename Y> Coord(Coord<dim, Y> v);

	// Initializer list constructor.
	Coord(std::initializer_list<T> const & a);

	// Returns a unit vector along the i axis.
	static Coord<dim, T> axis(unsigned int i);

	// Returns a vector with all elements equal to a.
	static Coord<dim, T> filled(T a);

	// Implicit conversion from one dimensional vector to type T
	operator T () const;

	// Access element at index i.
	T & operator [] (unsigned int i);

	// Access element at index i.
	T operator [] (unsigned int i) const;

	// Assignment operator. Each element in v is converted from type Y to type T.
	template <typename Y> Coord<dim, T> const & operator = (Coord<dim, Y> v);

	// Set the elements.
	Coord<dim, T> const & operator = (std::initializer_list<T> const & a);
	
	// Add v to this.
	void operator += (Coord<dim, T> v);

	// Subtract v from this.
	void operator -= (Coord<dim, T> v);

	// Multiply this by a.
	void operator *= (T a);

	// Normalize this.
	void normalize();

	// Get a pointer to the elements.
	T * ptr();

	// Get a pointer to the elements.
	T const * ptr() const;

	// Returns true if all of the elements are zero.
	bool isZero() const;

	// Returns this extended to a higher dimension newDim, filling the extra elements with fill.
	template <unsigned int newDim> Coord<newDim, T> extend(T fill) const;

	// Returns this shrunk to a lower dimension newDim.
	template <unsigned int newDim> Coord<newDim, T> shrink() const;

	// Returns the dot product of this with v.
	T dot(Coord<dim, T> v) const;

	// Returns the three-dimensional cross product of this and v. The vectors must be three dimensional.
	Coord<dim, T> cross(Coord<dim, T> v) const;

	// Returns this rotated by 90 degrees counter-clockwise. The vector must be two dimensional.
	Coord<dim, T> perp2d() const;

	// Returns the two dimensional cross product of this and v(abs(this) abs(v) sin(the angle between the vectors). The vectors must be two dimensional.
	T cross2d(Coord<dim, T> v) const;

	// Returns a vector that is this from the reference frame of v (as an x-axis) with a norm that is the product of the norms of this and v. The vectors must be two dimensional.
	Coord<dim, T> relative2d(Coord<dim, T> v) const;

	// Returns a vector rotated counter-clockwise by the angle a. The vector must be two dimensional.
	Coord<dim, T> rotate2d(float a);

	// Returns an arbitrary vector perpendicular to this. The result is of arbitrary norm. The vector must be three dimensional.
	Coord<dim, T> perpendicular() const;

	// Returns the norm/magnitude/length.
	T norm() const;

	// Returns the square of the norm/magnitude/length.
	T normSq() const;

	// Returns the unit vector of this. This must not be a zero vector.
	Coord<dim, T> unit() const;

	// Returns a vector with each element in this multiplied by the corresponding element in v.
	Coord<dim, T> scale(Coord<dim, T> v) const;

	// Returns a vector with each element in this divided by the corresponding element in v. V must not contain any zero elements.
	Coord<dim, T> scaleInv(Coord<dim, T> v) const;

	// Returns a vector with each element in this clamped to the range [min, max].
	Coord<dim, T> clamp(T min, T max) const;

	// Returns a vector with each element in this clamped to the range specificied by the corresponding elements in min and max.
	Coord<dim, T> clamp(Coord<dim, T> min, Coord<dim, T> max) const;

private:
	T c[dim];

	template <unsigned int dimY, typename Y> friend class Coord;
};

typedef Coord<2, int> Coord2i;
typedef Coord<3, int> Coord3i;
typedef Coord<4, int> Coord4i;
typedef Coord<2, float> Coord2f;
typedef Coord<3, float> Coord3f;
typedef Coord<4, float> Coord4f;
typedef Coord<2, double> Coord2d;
typedef Coord<3, double> Coord3d;
typedef Coord<4, double> Coord4d;

// Returns true if each element in v0 is equal to the corresponding element in v1.
template <unsigned int dim, typename T> bool operator == (Coord<dim, T> v0, Coord<dim, T> v1);

// Returns true if any element in v0 is not equal to the corresponding elment in v1.
template <unsigned int dim, typename T> bool operator != (Coord<dim, T> v0, Coord<dim, T> v1);

// Returns true if the the first element in v0 that is not equal to the corresponding element in v1 is less than the other element. If they are all equal, it returns false.
template <unsigned int dim, typename T> bool operator < (Coord<dim, T> v0, Coord<dim, T> v1);

// Returns -v.
template <unsigned int dim, typename T> Coord<dim, T> operator - (Coord<dim, T> const & v);

// Returns +v.
template <unsigned int dim, typename T> Coord<dim, T> operator + (Coord<dim, T> const & v);

// Returns v0 + v1.
template <unsigned int dim, typename T> Coord<dim, T> operator + (Coord<dim, T> v0, Coord<dim, T> v1);

// Returns v0 - v1.
template <unsigned int dim, typename T> Coord<dim, T> operator - (Coord<dim, T> v0, Coord<dim, T> v1);

// Returns a v.
template <unsigned int dim, typename T> Coord<dim, T> operator * (T a, Coord<dim, T> v);

// Returns v a.
template <unsigned int dim, typename T> Coord<dim, T> operator * (Coord<dim, T> v, T a);

// Returns v / a. Beware of truncation if they are both integers.
template <unsigned int dim, typename T> Coord<dim, T> operator / (Coord<dim, T> v, T a);

// Template implementations

template <unsigned int dim, typename T>
Coord<dim, T>::Coord()
{
	assert(dim > 0);
	for(unsigned int i = 0; i < dim; ++i)
	{
		c[i] = 0;
	}
}

template <unsigned int dim, typename T> template <typename Y>
Coord<dim, T>::Coord(Coord<dim, Y> v)
{
	assert(dim > 0);
	*this = a;
}

template <unsigned int dim, typename T>
Coord<dim, T>::Coord(std::initializer_list<T> const & a)
{
	assert(dim > 0);
	*this = a;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::axis(unsigned int i)
{
	if(i >= dim)
	{
		throw std::exception();
	}
	Coord<dim, T> r;
	for(unsigned int j = 0; j < dim; ++j)
	{
		r.c[j] = 0;
	}
	r.c[i] = 1;
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::filled(T a)
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r.c[i] = a;
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T>::operator T () const
{
	assert(dim == 1);
	return c[0];
}

template <unsigned int dim, typename T>
T & Coord<dim, T>::operator [] (unsigned int i)
{
	if(i >= dim)
	{
		throw std::exception();
	}
	return c[i];
}

template <unsigned int dim, typename T>
T Coord<dim, T>::operator [] (unsigned int i) const
{
	if(i >= dim)
	{
		throw std::exception();
	}
	return c[i];
}

template <unsigned int dim, typename T> template <typename Y>
Coord<dim, T> const & Coord<dim, T>::operator = (Coord<dim, Y> v)
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		c[i] = (T)v.c[i];
	}
	return *this;
}

template <unsigned int dim, typename T>
Coord<dim, T> const & Coord<dim, T>::operator = (std::initializer_list<T> const & a)
{
	if(a.size() != dim)
	{
		throw std::exception();
	}
	unsigned int i = 0;
	for(auto t : a)
	{
		c[i] = t;
		i++;
	}
	return *this;
}

template <unsigned int dim, typename T>
void Coord<dim, T>::operator += (Coord<dim, T> v)
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		c[i] += v.c[i];
	}
}

template <unsigned int dim, typename T>
void Coord<dim, T>::operator -= (Coord<dim, T> v)
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		c[i] -= v.c[i];
	}
}

template <unsigned int dim, typename T>
void Coord<dim, T>::operator *= (T a)
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		c[i] *= a;
	}
}

template <unsigned int dim, typename T>
void Coord<dim, T>::normalize()
{
	T n = norm();
	if(n == 0)
	{
		throw std::exception();
	}
	T nInv = 1 / n;
	for(unsigned int i = 0; i < dim; ++i)
	{
		c[i] = c[i] * nInv;
	}
}

template <unsigned int dim, typename T>
T * Coord<dim, T>::ptr()
{
	return c;
}

template <unsigned int dim, typename T>
T const * Coord<dim, T>::ptr() const
{
	return c;
}

template <unsigned int dim, typename T>
bool Coord<dim, T>::isZero() const
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		if(c[i] != 0)
		{
			return false;
		}
	}
	return true;
}

template <unsigned int dim, typename T> template <unsigned int newDim>
Coord<newDim, T> Coord<dim, T>::extend(T fill) const
{
	assert(newDim > dim);
	Coord<newDim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r[i] = c[i];
	}
	for(unsigned int i = dim; i < newDim; ++i)
	{
		r[i] = fill;
	}
	return r;
}

template <unsigned int dim, typename T> template <unsigned int newDim>
Coord<newDim, T> Coord<dim, T>::shrink() const
{
	assert(newDim <= dim);
	Coord<newDim, T> r;
	for(unsigned int i = 0; i < newDim; ++i)
	{
		r[i] = c[i];
	}
	return r;
}

template <unsigned int dim, typename T>
T Coord<dim, T>::dot(Coord<dim, T> v) const
{
	T r = 0;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r += c[i] * v.c[i];
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::cross(Coord<dim, T> v) const
{
	assert(dim == 3);
	Coord<dim, T> r;
	r.c[0] = c[1] * v.c[2] - c[2] * v.c[1];
	r.c[1] = c[2] * v.c[0] - c[0] * v.c[2];
	r.c[2] = c[0] * v.c[1] - c[1] * v.c[0];
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::perp2d() const
{
	assert(dim == 2);
	Coord<dim, T> r;
	r.c[0] = -c[1];
	r.c[1] = c[0];
	return r;
}

template <unsigned int dim, typename T>
T Coord<dim, T>::cross2d(Coord<dim, T> v) const
{
	assert(dim == 2);
	return c[0] * v.c[1] - c[1] * v.c[0];
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::relative2d(Coord<dim, T> v) const
{
	assert(dim == 2);
	return Coord<dim, T>(dot(v), cross2D(v));
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::rotate2d(float a)
{
	assert(dim == 2);
	T cosa = std::cos(a);
	T sina = std::sin(a);
	return Coord<dim, T>(c[0] * cosa - c[1] * sina, c[0] * sina + c[1] * cosa);
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::perpendicular() const
{
	assert(dim == 3);
	Coord<dim, T> r(0, c[2], -c[1]);
	if(c[2] == 0 && c[1] == 0)
	{
		r[1] = c[0];
	}
	return r;
}

template <unsigned int dim, typename T>
T Coord<dim, T>::norm() const
{
	T r = dot(*this);
	return sqrt(r);
}

template <unsigned int dim, typename T>
T Coord<dim, T>::normSq() const
{
	return dot(*this);
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::unit() const
{
	T n = norm();
	if(n == 0)
	{
		throw std::exception();
	}
	T nInv = 1 / n;
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r.c[i] = c[i] * nInv;
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::scale(Coord<dim, T> v) const
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r.c[i] = c[i] * v.c[i];
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::scaleInv(Coord<dim, T> v) const
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		if(v.c[i] == 0)
		{
			throw std::exception();
		}
		r.c[i] = c[i] / v.c[i];
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::clamp(T min, T max) const
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		if(c[i] < min)
		{
			r.c[i] = min;
		}
		else if(c[i] > max)
		{
			r.c[i] = max;
		}
		else
		{
			r.c[i] = c[i];
		}
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> Coord<dim, T>::clamp(Coord<dim, T> min, Coord<dim, T> max) const
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		if(c[i] < min.c[i])
		{
			r.c[i] = min.c[i];
		}
		else if(c[i] > max.c[i])
		{
			r.c[i] = max.c[i];
		}
		else
		{
			r.c[i] = c[i];
		}
	}
	return r;
}

template <unsigned int dim, typename T>
bool operator == (Coord<dim, T> v0, Coord<dim, T> v1)
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		if(v0[i] != v1[i])
		{
			return false;
		}
	}
	return true;
}

template <unsigned int dim, typename T>
bool operator != (Coord<dim, T> v0, Coord<dim, T> v1)
{
	return !(v0 == v1);
}

template <unsigned int dim, typename T>
bool operator < (Coord<dim, T> v0, Coord<dim, T> v1)
{
	for(unsigned int i = 0; i < dim; ++i)
	{
		if(v0[i] < v1[i])
		{
			return true;
		}
		else if(v0[i] > v1[i])
		{
			return false;
		}
	}
	return false; // They are equal, so it is false.
}

template <unsigned int dim, typename T>
Coord<dim, T> operator - (Coord<dim, T> const & v)
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r[i] = -v[i];
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> operator + (Coord<dim, T> const & v)
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r[i] = +v[i];
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> operator + (Coord<dim, T> v0, Coord<dim, T> v1)
{
	Coord<dim, T> r(v0);
	r += v1;
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> operator - (Coord<dim, T> v0, Coord<dim, T> v1)
{
	Coord<dim, T> r(v0);
	r -= v1;
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> operator * (T a, Coord<dim, T> v)
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r[i] = a * v[i];
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> operator * (Coord<dim, T> v, T a)
{
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r[i] = v[i] * a;
	}
	return r;
}

template <unsigned int dim, typename T>
Coord<dim, T> operator / (Coord<dim, T> v, T a)
{
	if(a == 0)
	{
		throw std::exception();
	}
	Coord<dim, T> r;
	for(unsigned int i = 0; i < dim; ++i)
	{
		r[i] = v[i] / a;
	}
	return r;
}

