#pragma once

#include "vector.h"

namespace kit
{
	//! A <b>dim</b>-dimensional interval with dimensions of type \b T.
	template <int dim, typename T>
	class Interval
	{
	public:
		//! Default constructor.
		Interval();

		//! Returns a box given a \b min and \b max.
		static Interval<dim, T> minMax(Vector<dim, T> min, Vector<dim, T> max);

		//! Returns a 2D box given(\b minX, \b minY) and(\b maxX, \b maxY) coordinates.
		static Interval<2, T> minMax(T minX, T minY, T maxX, T maxY);

		//! Returns a 3D box given(\b minX, \b minY, \b minZ) and(\b maxX, \b maxY, \b maxZ) coordinates.
		static Interval<3, T> minMax(T minX, T minY, T minZ, T maxX, T maxY, T maxZ);

		//! Returns a box given a \b min and a \b size.
		static Interval<dim, T> minSize(Vector<dim, T> min, Vector<dim, T> size);

		//! Returns a 2D box given(\b minX, \b minY) coordinates and(\b sizeX, \b sizeY) size.
		static Interval<2, T> minSize(T minX, T minY, T sizeX, T sizeY);

		//! Returns a 3D box given(\b minX, \b minY, \b minZ) coordinates and(\b sizeX, \b sizeY, \b sizeZ) size.
		static Interval<3, T> minSize(T minX, T minY, T minZ, T sizeX, T sizeY, T sizeZ);

		//! Returns the size of the box.
		Vector<dim, T> getSize() const;

		//! Sets the min to \b min and maintains the size by adjusting max.
		void setMinKeepSize(Vector<dim, T> min);

		//! Sets the max to min + \b size.
		void setSize(Vector<dim, T> size);

		//! Makes the max and min to extend around p.
		void extendTo(Vector<dim, T> p);

		//! Returns true if every element v[i] in \b v is within [min, max].
		bool containsInc(Vector<dim, T> v) const;

		//! Returns true if every element v[i] in \b v is within [min, max).
		bool containsEx(Vector<dim, T> v) const;

		//! Returns true if any part of b is within(not just touching) the box.
		bool intersects(Interval<dim, T> b) const;

		//! Returns a position or size aligned to the interval.
		Vector<dim, T> getAligned(Vector<dim, float> fractionOfIntervalSize, Vector<dim, T> offset);

		//! Returns an object position aligned to the interval, given the size of an object.
		Vector<dim, T> getAligned(Vector<dim, T> objectSize, Vector<dim, float> fractionOfObjectSize, Vector<dim, float> fractionOfIntervalSize, Vector<dim, T> offset);

		Vector<dim, T> min;
		Vector<dim, T> max;
	};

	template <int dim, typename T>
	Interval<dim, T>::Interval()
	{
	}

	template <int dim, typename T>
	Interval<dim, T> Interval<dim, T>::minMax(Vector<dim, T> min, Vector<dim, T> max)
	{
		Interval<dim, T> box;
		box.min = min;
		box.max = max;
		return box;
	}

	template <int dim, typename T>
	Interval<2, T> Interval<dim, T>::minMax(T minX, T minY, T maxX, T maxY)
	{
		Interval<2, T> box;
		box.min.set(minX, minY);
		box.max.set(maxX, maxX);
		return box;
	}

	template <int dim, typename T>
	Interval<3, T> Interval<dim, T>::minMax(T minX, T minY, T minZ, T maxX, T maxY, T maxZ)
	{
		Interval<3, T> box;
		box.min.set(minX, minY, minZ);
		box.max.set(maxX, maxY, maxZ);
		return box;
	}

	template <int dim, typename T>
	Interval<dim, T> Interval<dim, T>::minSize(Vector<dim, T> min, Vector<dim, T> size)
	{
		Interval<dim, T> box;
		box.min = min;
		box.max = min + size;
		return box;
	}

	template <int dim, typename T>
	Interval<2, T> Interval<dim, T>::minSize(T minX, T minY, T sizeX, T sizeY)
	{
		Interval<2, T> box;
		box.min.set(minX, minY);
		box.max.set(minX + sizeX, minY + sizeY);
		return box;
	}

	template <int dim, typename T>
	Interval<3, T> Interval<dim, T>::minSize(T minX, T minY, T minZ, T sizeX, T sizeY, T sizeZ)
	{
		Interval<2, T> box;
		box.min.set(minX, minY, minZ);
		box.max.set(minX + sizeX, minY + sizeY, minZ + sizeZ);
		return box;
	}

	template <int dim, typename T>
	Vector<dim, T> Interval<dim, T>::getSize() const
	{
		return max - min;
	}

	template <int dim, typename T>
	void Interval<dim, T>::setMinKeepSize(Vector<dim, T> min)
	{
		max += min - this->min;
		this->min = min;
	}

	template <int dim, typename T>
	void Interval<dim, T>::setSize(Vector<dim, T> size)
	{
		max = min + size;
	}

	template <int dim, typename T>
	void Interval<dim, T>::extendTo(Vector<dim, T> p)
	{
		for(int i = 0; i < dim; ++i)
		{
			if(min[i] > p[i])
			{
				min[i] = p[i];
			}
			else if(max[0] < p[i])
			{
				max[i] = p[i];
			}
		}
	}

	template <int dim, typename T>
	bool Interval<dim, T>::containsInc(Vector<dim, T> v) const
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
	bool Interval<dim, T>::containsEx(Vector<dim, T> v) const
	{
		for(int i = 0; i < dim; ++i)
		{
			if(v[i] < min[i] || max[i] <= v[i])
			{
				return false;
			}
		}
		return true;
	}

	template <int dim, typename T>
	bool Interval<dim, T>::intersects(Interval<dim, T> b) const
	{
		for(int i = 0; i < dim; ++i)
		{
			if(min[i] >= b.max[i] || max[i] <= b.min[i])
			{
				return false;
			}
		}
		return true;
	}

	template <int dim, typename T>
	Vector<dim, T> Interval<dim, T>::getAligned(Vector<dim, float> fractionOfIntervalSize, Vector<dim, T> offset)
	{
		Vector<dim, T> r;
		for(int i = 0; i < dim; ++i)
		{
			r[i] = min[i] + offset[i] + (T)((max[i] - min[i]) * fractionOfIntervalSize[i]);
		}
		return r;
	}

	template <int dim, typename T>
	Vector<dim, T> Interval<dim, T>::getAligned(Vector<dim, T> objectSize, Vector<dim, float> fractionOfObjectSize, Vector<dim, float> fractionOfIntervalSize, Vector<dim, T> offset)
	{
		Vector<dim, T> r;
		for(int i = 0; i < dim; ++i)
		{
			r[i] = min[i] + offset[i] + (T)((max[i] - min[i]) * fractionOfIntervalSize[i]) - (T)(objectSize[i] * fractionOfObjectSize[i]);
		}
		return r;
	}
}

