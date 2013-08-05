#pragma once

#include "Vector.h"

//! A <b>dim</b>-dimensional rectangle with dimensions of type \b T.
template <int dim, typename T>
class Box
{
public:
  //! Default constructor.
  Box ();

  //! Returns a box with all values at zero.
  static Box<dim, T> zero ();

  //! Returns a box given a \b min and \b max.
  static Box<dim, T> minMax (Vector<dim, T> min, Vector<dim, T> max);

  //! Returns a 2D box given (\b minX, \b minY) and (\b maxX, \b maxY) coordinates.
  static Box<2, T> minMax (T minX, T minY, T maxX, T maxY);

  //! Returns a 3D box given (\b minX, \b minY, \b minZ) and (\b maxX, \b maxY, \b maxZ) coordinates.
  static Box<3, T> minMax (T minX, T minY, T minZ, T maxX, T maxY, T maxZ);

  //! Returns a box given a \b min and a \b size.
  static Box<dim, T> minSize (Vector<dim, T> min, Vector<dim, T> size);

  //! Returns a 2D box given (\b minX, \b minY) coordinates and (\b sizeX, \b sizeY) size.
  static Box<2, T> minSize (T minX, T minY, T sizeX, T sizeY);

  //! Returns a 3D box given (\b minX, \b minY, \b minZ) coordinates and (\b sizeX, \b sizeY, \b sizeZ) size.
  static Box<3, T> minSize (T minX, T minY, T minZ, T sizeX, T sizeY, T sizeZ);

  //! Returns the size of the box.
  Vector<dim, T> getSize () const;

  //! Sets the min to \b min and maintains the size by adjusting max.
  void setMinKeepSize (Vector<dim, T> min);

  //! Sets the max to min + \b size.
  void setSize (Vector<dim, T> size);

  //! Returns true if every element v[i] in \b v is within [min, max].
  bool containsInc (Vector<dim, T> v) const;

  //! Returns true if every element v[i] in \b v is within [min, max).
  bool containsEx (Vector<dim, T> v) const;

  //! Returns a position or size aligned to the box.
  Vector<dim, T> getAligned (Vector<dim, float> fractionOfBoxSize, Vector<dim, T> offset);

  //! Returns an object position aligned to the box, given the size of an object.
  Vector<dim, T> getAligned (Vector<dim, T> objectSize, Vector<dim, float> fractionOfObjectSize, Vector<dim, float> fractionOfBoxSize, Vector<dim, T> offset);

  Vector<dim, T> min;
  Vector<dim, T> max;
};

typedef Box<2, int> Box2i;
typedef Box<3, int> Box3i;
typedef Box<2, float> Box2f;
typedef Box<3, float> Box3f;

template <int dim, typename T>
Box<dim, T>::Box ()
{
}

template <int dim, typename T>
Box<dim, T> Box<dim, T>::zero ()
{
  Box<dim, T> box;
  box.min = Vector<dim, T>::zero();
  box.max = Vector<dim, T>::zero();
  return box;
}

template <int dim, typename T>
Box<dim, T> Box<dim, T>::minMax (Vector<dim, T> min, Vector<dim, T> max)
{
  Box<dim, T> box;
  box.min = min;
  box.max = max;
  return box;
}

template <int dim, typename T>
Box<2, T> Box<dim, T>::minMax (T minX, T minY, T maxX, T maxY)
{
  Box<2, T> box;
  box.min.set(minX, minY);
  box.max.set(maxX, maxX);
  return box;
}

template <int dim, typename T>
Box<3, T> Box<dim, T>::minMax (T minX, T minY, T minZ, T maxX, T maxY, T maxZ)
{
  Box<3, T> box;
  box.min.set(minX, minY, minZ);
  box.max.set(maxX, maxY, maxZ);
  return box;
}

template <int dim, typename T>
Box<dim, T> Box<dim, T>::minSize (Vector<dim, T> min, Vector<dim, T> size)
{
  Box<dim, T> box;
  box.min = min;
  box.max = min + size;
  return box;
}

template <int dim, typename T>
Box<2, T> Box<dim, T>::minSize (T minX, T minY, T sizeX, T sizeY)
{
  Box<2, T> box;
  box.min.set(minX, minY);
  box.max.set(minX + sizeX, minY + sizeY);
  return box;
}

template <int dim, typename T>
Box<3, T> Box<dim, T>::minSize (T minX, T minY, T minZ, T sizeX, T sizeY, T sizeZ)
{
  Box<2, T> box;
  box.min.set(minX, minY, minZ);
  box.max.set(minX + sizeX, minY + sizeY, minZ + sizeZ);
  return box;
}

template <int dim, typename T>
Vector<dim, T> Box<dim, T>::getSize () const
{
  return max - min;
}

template <int dim, typename T>
void Box<dim, T>::setMinKeepSize (Vector<dim, T> min)
{
  max += min - this->min;
  this->min = min;
}

template <int dim, typename T>
void Box<dim, T>::setSize (Vector<dim, T> size)
{
  max = min + size;
}

template <int dim, typename T>
bool Box<dim, T>::containsInc (Vector<dim, T> v) const
{
  for (int i = 0; i < dim; ++i)
  {
    if (v[i] < min[i] || max[i] < v[i])
    {
      return false;
    }
  }
  return true;
}

template <int dim, typename T>
bool Box<dim, T>::containsEx (Vector<dim, T> v) const
{
  for (int i = 0; i < dim; ++i)
  {
    if (v[i] < min[i] || max[i] <= v[i])
    {
      return false;
    }
  }
  return true;
}

template <int dim, typename T>
Vector<dim, T> Box<dim, T>::getAligned (Vector<dim, float> fractionOfBoxSize, Vector<dim, T> offset)
{
  Vector<dim, T> r;
  for (int i = 0; i < dim; ++i)
  {
    r[i] = min[i] + offset[i] + (T)((max[i] - min[i]) * fractionOfBoxSize[i]);
  }
  return r;
}

template <int dim, typename T>
Vector<dim, T> Box<dim, T>::getAligned (Vector<dim, T> objectSize, Vector<dim, float> fractionOfObjectSize, Vector<dim, float> fractionOfBoxSize, Vector<dim, T> offset)
{
  Vector<dim, T> r;
  for (int i = 0; i < dim; ++i)
  {
    r[i] = min[i] + offset[i] + (T)((max[i] - min[i]) * fractionOfBoxSize[i]) - (T)(objectSize[i] * fractionOfObjectSize[i]);
  }
  return r;
}

