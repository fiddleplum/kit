#pragma once

#include "vector.h"

// A interval with dimensions of type T.
template <int dim, typename T>
class Interval
{
public:
	// Constructs to all zeros.
	Interval();

	// Constructs to the other.
	Interval(Interval<dim, T> const & other);

	// Constructs to min and max.
	Interval(Vector<dim, T> min, Vector<dim, T> max);

	// Returns true if every element in v is within the corresponding dimension of the interval.
	bool contains(Vector<dim, T> v) const;

	// Returns true if any part of the interval is within the other and vice versa.
	bool intersects(Interval<dim, T> other) const;

	// Returns the point closest to p within within the interval.
	Vector<dim, T> closest(Vector<dim, T> p) const;

	// Returns an interval that is the interval extended around p by decreasing the min or increasing the max, if necessary.
	Interval<dim, T> extendedTo(Vector<dim, T> p) const;

	// Returns an interval that is the union of this and other.
	Interval<dim, T> unionedWith(Interval<dim, T> const & other) const;

	// Returns an interval that is the intersection of this and other. If they do not overlap, the result is all zeros.
	Interval<dim, T> intersectedWith(Interval<dim, T> const & other) const;

	// Returns a position or size aligned to the interval.
	template <typename Y> Vector<dim, T> getAligned(Vector<dim, Y> fractionOfThisSize, Vector<dim, T> offset) const;

	// Returns an object position aligned to the interval, given the size of an object. You may want to set the size of the object first using the function above.
	template <typename Y> Vector<dim, T> getAligned(Vector<dim, T> objectSize, Vector<dim, Y> fractionOfObjectSize, Vector<dim, Y> fractionOfThisSize, Vector<dim, T> offset) const;

	Vector<dim, T> min;
	Vector<dim, T> max;
};

template <int dim, typename T>
Interval<dim, T>::Interval()
{
}

template <int dim, typename T>
Interval<dim, T>::Interval(Interval<dim, T> const & other)
{
	min = other.min;
	max = other.max;
}

template <int dim, typename T>
Interval<dim, T>::Interval(Vector<dim, T> min_, Vector<dim, T> max_)
{
	min = min_;
	max = max_;
}

template <int dim, typename T>
bool Interval<dim, T>::contains(Vector<dim, T> v) const
{
	for(int i = 0; i < dim; ++i)
	{
		if(v[i] < min[i] || max[i] < v[i])
		{
			return false;
		}
	}
	return true;
}

template <int dim, typename T>
bool Interval<dim, T>::intersects(Interval<dim, T> other) const
{
	for(int i = 0; i < dim; ++i)
	{
		if(other.max[i] < min[i] || max[i] < other.min[i])
		{
			return false;
		}
	}
	return true;
}

template <int dim, typename T>
Vector<dim, T> Interval<dim, T>::closest(Vector<dim, T> p) const
{
	Vector<dim, T> r;
	for(int i = 0; i < dim; ++i)
	{
		if(p[i] < min[i])
		{
			r[i] = min[i];
		}
		else if(max[i] < p[i])
		{
			r[i] = max[i];
		}
		else
		{
			r[i] = p[i];
		}
	}
	return r;
}

template <int dim, typename T>
Interval<dim, T> Interval<dim, T>::extendedTo(Vector<dim, T> p) const
{
	Interval<dim, T> r;
	for(int i = 0; i < dim; ++i)
	{
		r.min[i] = (p[i] < min[i] ? p[i] : min[i]);
		r.max[i] = (max[i] < p[i] ? p[i] : max[i]);
	}
	return r;
}

template <int dim, typename T>
Interval<dim, T> Interval<dim, T>::unionedWith(Interval<dim, T> const & other) const
{
	Interval<dim, T> r;
	for(int i = 0; i < dim; ++i)
	{
		r.min[i] = (other.min[i] < min[i] ? other.min[i] : min[i]);
		r.max[i] = (max[i] < other.max[i] ? other.max[i] : max[i]);
	}
	return r;
}

template <int dim, typename T>
Interval<dim, T> Interval<dim, T>::intersectedWith(Interval<dim, T> const & other) const
{
	Interval<dim, T> r;
	for(int i = 0; i < dim; ++i)
	{
		r.min[i] = (other.min[i] < min[i] ? min[i] : other.min[i]);
		r.max[i] = (max[i] < other.max[i] ? max[i] : other.max[i]);
		if(r.max[i] < r.min[i])
		{
			r.min[i] = r.max[i] = 0;
		}
	}
	return r;
}

template <int dim, typename T> template <typename Y>
Vector<dim, T> Interval<dim, T>::getAligned(Vector<dim, Y> fractionOfThisSize, Vector<dim, T> offset) const
{
	Vector<dim, T> r;
	for(int i = 0; i < dim; ++i)
	{
		r[i] = min[i] + offset[i] + (T)((max[i] - min[i]) * fractionOfThisSize[i]);
	}
	return r;
}

template <int dim, typename T> template <typename Y>
Vector<dim, T> Interval<dim, T>::getAligned(Vector<dim, T> objectSize, Vector<dim, Y> fractionOfObjectSize, Vector<dim, Y> fractionOfThisSize, Vector<dim, T> offset) const
{
	Vector<dim, T> r;
	for(int i = 0; i < dim; ++i)
	{
		r[i] = min[i] + offset[i] + (T)((max[i] - min[i]) * fractionOfThisSize[i]) - (T)(objectSize[i] * fractionOfObjectSize[i]);
	}
	return r;
}

