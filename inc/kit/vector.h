#pragma once

#include <exception>
#include <cassert>
#include <cmath>

namespace kit
{
	//! This is a standard mathematical vector class. Dim is the dimensions of the vector and T is the type of its elements.
	template <unsigned int dim, typename T>
	class Vector
	{
	public:
		//! Default constructor. Zeroes all elements.
		Vector ();

		//! Constructor for a one dimensional vector.
		Vector (T a0);

		//! Constructor for a two dimensional vector.
		Vector (T a0, T a1);

		//! Constructor for a three dimensional vector.
		Vector (T a0, T a1, T a2);

		//! Constructor for a four dimensional vector.
		Vector (T a0, T a1, T a2, T a3);

		//! Copy constructor. Each element in v is converted from type Y to type T.
		template <typename Y> Vector (Vector<dim, Y> v);

		//! Returns a unit vector along the i axis.
		static Vector<dim, T> axis (unsigned int i);

		//! Returns a vector with all elements equal to a.
		static Vector<dim, T> filled (T a);

		//! Implicit conversion from one dimensional vector to type T
		operator T ();

		//! Access element at index i.
		T & operator [] (unsigned int i);

		//! Access element at index i.
		T operator [] (unsigned int i) const;
	
		//! Assignment operator. Each element in v is converted from type Y to type T.
		template <typename Y> Vector<dim, T> operator = (Vector<dim, Y> v);

		//! Set the elements for a one dimensional vector.
		void set (T a0);

		//! Set the elements for a two dimensional vector.
		void set (T a0, T a1);

		//! Set the elements for a three dimensional vector.
		void set (T a0, T a1, T a2);

		//! Set the elements for a four dimensional vector.
		void set (T a0, T a1, T a2, T a3);

		//! Add v to this.
		void operator += (Vector<dim, T> v);
	
		//! Subtract v from this.
		void operator -= (Vector<dim, T> v);
	
		//! Multiply this by a.
		void operator *= (T a);

		//! Normalize this.
		void normalize ();
	
		//! Get a pointer to the elements.
		T * ptr ();
	
		//! Get a pointer to the elements.
		T const * ptr () const;

		//! Returns true if all of the elements are zero.
		bool isZero () const;

		//! Returns this extended to a higher dimension newDim, filling the extra elements with fill.
		template <unsigned int newDim> Vector<newDim, T> extend (T fill) const;

		//! Returns this shrunk to a lower dimension newDim.
		template <unsigned int newDim> Vector<newDim, T> shrink () const;

		//! Returns the dot product of this with v.
		T dot (Vector<dim, T> v) const;

		//! Returns the three-dimensional cross product of this and v. The vectors must be three dimensional.
		Vector<dim, T> cross (Vector<dim, T> v) const;
	
		//! Returns this rotated by 90 degrees counter-clockwise. The vector must be two dimensional.
		Vector<dim, T> perp2d () const;
	
		//! Returns the two dimensional cross product of this and v(abs(this) abs(v) sin(the angle between the vectors). The vectors must be two dimensional.
		T cross2d (Vector<dim, T> v) const;
	
		//! Returns a vector that is this from the reference frame of v (as an x-axis) with a norm that is the product of the norms of this and v. The vectors must be two dimensional.
		Vector<dim, T> relative2d (Vector<dim, T> v) const;

		//! Returns a vector rotated counter-clockwise by the angle a. The vector must be two dimensional.
		Vector<dim, T> rotate2d (float a);

		//! Returns an arbitrary vector perpendicular to this. The result is of arbitrary norm. The vector must be three dimensional.
		Vector<dim, T> perpendicular () const;
	
		//! Returns the norm/magnitude/length.
		T norm () const;

		//! Returns the square of the norm/magnitude/length.
		T normSq () const;

		//! Returns the unit vector of this. This must not be a zero vector.
		Vector<dim, T> unit () const;

		//! Returns a vector with each element in this multiplied by the corresponding element in v.
		Vector<dim, T> scale (Vector<dim, T> v) const;

		//! Returns a vector with each element in this divided by the corresponding element in v. V must not contain any zero elements.
		Vector<dim, T> scaleInv (Vector<dim, T> v) const;

		//! Returns a vector with each element in this clamped to the range [min, max].
		Vector<dim, T> clamp (T min, T max) const;

		//! Returns a vector with each element in this clamped to the range specificied by the corresponding elements in min and max.
		Vector<dim, T> clamp (Vector<dim, T> min, Vector<dim, T> max) const;

	private:
		T c [dim];

		template <unsigned int dimY, typename Y> friend class Vector;
	};

	typedef Vector<2, int> Vector2i;
	typedef Vector<3, int> Vector3i;
	typedef Vector<4, int> Vector4i;
	typedef Vector<2, float> Vector2f;
	typedef Vector<3, float> Vector3f;
	typedef Vector<4, float> Vector4f;
	typedef Vector<2, double> Vector2d;
	typedef Vector<3, double> Vector3d;
	typedef Vector<4, double> Vector4d;

	//! Returns true if each element in v0 is equal to the corresponding element in v1.
	template <unsigned int dim, typename T> bool operator == (Vector<dim, T> v0, Vector<dim, T> v1);

	//! Returns true if any element in v0 is not equal to the corresponding elment in v1.
	template <unsigned int dim, typename T> bool operator != (Vector<dim, T> v0, Vector<dim, T> v1);

	//! Returns true if the the first element in v0 that is not equal to the corresponding element in v1 is less than the other element. If they are all equal, it returns false.
	template <unsigned int dim, typename T> bool operator < (Vector<dim, T> v0, Vector<dim, T> v1);

	//! Returns -v.
	template <unsigned int dim, typename T> Vector<dim, T> operator - (Vector<dim, T> const & v);

	//! Returns +v.
	template <unsigned int dim, typename T> Vector<dim, T> operator + (Vector<dim, T> const & v);

	//! Returns v0 + v1.
	template <unsigned int dim, typename T> Vector<dim, T> operator + (Vector<dim, T> v0, Vector<dim, T> v1);

	//! Returns v0 - v1.
	template <unsigned int dim, typename T> Vector<dim, T> operator - (Vector<dim, T> v0, Vector<dim, T> v1);

	//! Returns a v.
	template <unsigned int dim, typename T> Vector<dim, T> operator * (T a, Vector<dim, T> v);

	//! Returns v a.
	template <unsigned int dim, typename T> Vector<dim, T> operator * (Vector<dim, T> v, T a);

	//! Returns v / a. Beware of truncation if they are both integers.
	template <unsigned int dim, typename T> Vector<dim, T> operator / (Vector<dim, T> v, T a);

	// Template implementations

	template <unsigned int dim, typename T>
	Vector<dim, T>::Vector ()
	{
		assert(dim > 0);
		for(unsigned int i = 0; i < dim; ++i)
		{
			c[i] = 0;
		}
	}

	template <unsigned int dim, typename T>
	Vector<dim, T>::Vector (T a0)
	{
		assert(dim == 1);
		set(a0);
	}

	template <unsigned int dim, typename T>
	Vector<dim, T>::Vector (T a0, T a1)
	{
		assert(dim == 2);
		set(a0, a1);
	}

	template <unsigned int dim, typename T>
	Vector<dim, T>::Vector (T a0, T a1, T a2)
	{
		assert(dim == 3);
		set(a0, a1, a2);
	}

	template <unsigned int dim, typename T>
	Vector<dim, T>::Vector (T a0, T a1, T a2, T a3)
	{
		assert(dim == 4);
		set(a0, a1, a2, a3);
	}

	template <unsigned int dim, typename T> template <typename Y>
	Vector<dim, T>::Vector (Vector<dim, Y> v)
	{
		assert(dim > 0);
		for(unsigned int i = 0; i < dim; ++i)
		{
			c[i] =(T)v[i];
		}
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::axis (unsigned int i)
	{
		if(i >= dim)
		{
			throw std::exception ();
		}
		Vector<dim, T> r;
		for(unsigned int j = 0; j < dim; ++j)
		{
			r.c[j] = 0;
		}
		r.c[i] = 1;
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::filled (T a)
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r.c[i] = a;
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T>::operator T ()
	{
		assert(dim == 1);
		return c[0];
	}

	template <unsigned int dim, typename T>
	T & Vector<dim, T>::operator [] (unsigned int i)
	{
		if(i >= dim)
		{
			throw std::exception ();
		}
		return c[i];
	}

	template <unsigned int dim, typename T>
	T Vector<dim, T>::operator [] (unsigned int i) const
	{
		if(i >= dim)
		{
			throw std::exception ();
		}
		return c[i];
	}

	template <unsigned int dim, typename T> template <typename Y>
	Vector<dim, T> Vector<dim, T>::operator = (Vector<dim, Y> v)
	{
		for(unsigned int i = 0; i < dim; ++i)
		{
			c[i] =(T)v.c[i];
		}
		return *this;
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::set (T a0)
	{
		assert(dim == 1);
		c[0] = a0;
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::set (T a0, T a1)
	{
		assert(dim == 2);
		c[0] = a0;
		c[1] = a1;
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::set (T a0, T a1, T a2)
	{
		assert(dim == 3);
		c[0] = a0;
		c[1] = a1;
		c[2] = a2;
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::set (T a0, T a1, T a2, T a3)
	{
		assert(dim == 4);
		c[0] = a0;
		c[1] = a1;
		c[2] = a2;
		c[3] = a3;
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::operator += (Vector<dim, T> v)
	{
		for(unsigned int i = 0; i < dim; ++i)
		{
			c[i] += v.c[i];
		}
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::operator -= (Vector<dim, T> v)
	{
		for(unsigned int i = 0; i < dim; ++i)
		{
			c[i] -= v.c[i];
		}
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::operator *= (T a)
	{
		for(unsigned int i = 0; i < dim; ++i)
		{
			c[i] *= a;
		}
	}

	template <unsigned int dim, typename T>
	void Vector<dim, T>::normalize ()
	{
		T n = norm();
		if(n == 0)
		{
			throw std::exception ();
		}
		T nInv = 1 / n;
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r.c[i] = c[i] * nInv;
		}
	}

	template <unsigned int dim, typename T>
	T * Vector<dim, T>::ptr ()
	{
		return c;
	}

	template <unsigned int dim, typename T>
	T const * Vector<dim, T>::ptr () const
	{
		return c;
	}

	template <unsigned int dim, typename T>
	bool Vector<dim, T>::isZero () const
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
	Vector<newDim, T> Vector<dim, T>::extend (T fill) const
	{
		assert(newDim > dim);
		Vector<newDim, T> r;
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
	Vector<newDim, T> Vector<dim, T>::shrink () const
	{
		assert(newDim <= dim);
		Vector<newDim, T> r;
		for(unsigned int i = 0; i < newDim; ++i)
		{
			r[i] = c[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	T Vector<dim, T>::dot (Vector<dim, T> v) const
	{
		T r = 0;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r += c[i] * v.c[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::cross (Vector<dim, T> v) const
	{
		assert(dim == 3);
		Vector<dim, T> r;
		r.c[0] = c[1] * v.c[2] - c[2] * v.c[1];
		r.c[1] = c[2] * v.c[0] - c[0] * v.c[2];
		r.c[2] = c[0] * v.c[1] - c[1] * v.c[0];
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::perp2d () const
	{
		assert(dim == 2);
		Vector<dim, T> r;
		r.c[0] = -c[1];
		r.c[1] = c[0];
		return r;
	}

	template <unsigned int dim, typename T>
	T Vector<dim, T>::cross2d (Vector<dim, T> v) const
	{
		assert(dim == 2);
		return c[0] * v.c[1] - c[1] * v.c[0];
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::relative2d (Vector<dim, T> v) const
	{
		assert(dim == 2);
		return Vector<dim, T>(dot(v), cross2D(v));
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::rotate2d (float a)
	{
		assert(dim == 2);
		T cosa = std::cos(a);
		T sina = std::sin(a);
		return Vector<dim, T>(c[0] * cosa - c[1] * sina, c[0] * sina + c[1] * cosa);
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::perpendicular () const
	{
		assert(dim == 3);
		Vector<dim, T> r(0, c[2], -c[1]);
		if(c[2] == 0 && c[1] == 0)
		{
			r[1] = c[0];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	T Vector<dim, T>::norm () const
	{
		T r = dot(*this);
		return sqrt(r);
	}

	template <unsigned int dim, typename T>
	T Vector<dim, T>::normSq () const
	{
		return dot(*this);
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::unit () const
	{
		T n = norm();
		if(n == 0)
		{
			throw std::exception ();
		}
		T nInv = 1 / n;
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r.c[i] = c[i] * nInv;
		} 
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::scale (Vector<dim, T> v) const
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r.c[i] = c[i] * v.c[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::scaleInv (Vector<dim, T> v) const
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			if(v.c[i] == 0)
			{
				throw std::exception ();
			}
			r.c[i] = c[i] / v.c[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> Vector<dim, T>::clamp (T min, T max) const
	{
		Vector<dim, T> r;
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
	Vector<dim, T> Vector<dim, T>::clamp (Vector<dim, T> min, Vector<dim, T> max) const
	{
		Vector<dim, T> r;
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
	bool operator == (Vector<dim, T> v0, Vector<dim, T> v1)
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
	bool operator != (Vector<dim, T> v0, Vector<dim, T> v1)
	{
		return !(v0 == v1);
	}

	template <unsigned int dim, typename T>
	bool operator < (Vector<dim, T> v0, Vector<dim, T> v1)
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
	Vector<dim, T> operator - (Vector<dim, T> const & v)
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r[i] = -v[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> operator + (Vector<dim, T> const & v)
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r[i] = +v[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> operator + (Vector<dim, T> v0, Vector<dim, T> v1)
	{
		Vector<dim, T> r(v0);
		r += v1;
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> operator - (Vector<dim, T> v0, Vector<dim, T> v1)
	{
		Vector<dim, T> r(v0);
		r -= v1;
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> operator * (T a, Vector<dim, T> v)
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r[i] = a * v[i];
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> operator * (Vector<dim, T> v, T a)
	{
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r[i] = v[i] * a;
		}
		return r;
	}

	template <unsigned int dim, typename T>
	Vector<dim, T> operator / (Vector<dim, T> v, T a)
	{
		if(a == 0)
		{
			throw std::exception ();
		}
		Vector<dim, T> r;
		for(unsigned int i = 0; i < dim; ++i)
		{
			r[i] = v[i] / a;
		}
		return r;
	}
}

