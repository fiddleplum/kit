#pragma once

#include <cmath>
#include <cassert>
#include <cstdlib>

namespace Math
{
  //! Returns the constant pi.
  template <class T> T getPi ();

  //! Returns v clamped to between min and max.
  template <class T> T clamp (T v, T min, T max);

  //! Returns the maximum of x and y
  template <class T> T max (T x, T y);

  //! Returns the minimum of x and y
  template <class T> T min (T x, T y);

  //! Returns an int random number between min (inclusive) and max (exclusive).
  inline int random (int min, int max);

  //! Returns a float random number between min (inclusive) and max (inclusive). Has a resolution of (max - min) / RAND_MAX.
  inline float random (float min, float max);

  //! Returns the next highest power of two, given a 16 bit number.
  template <class T> T ceilPow2 (T x);

  //! Returns the ceiling of the integer, given the period.
  inline int ceil (int i, int period);

  // Template Implementations.

  template <class T> T getPi ()
  {
    return (T)3.141592653589793238462643383;
  }

  template <class T> T clamp (T v, T min, T max)
  {
    return v < min ? min : (v > max ? max : v);
  }

  template <class T> T max (T x, T y)
  {
    return x >= y ? x : y;
  }

  template <class T> T min (T x, T y)
  {
    return x <= y ? x : y;
  }

  inline int random (int min, int max)
  {
    assert(max - min <= RAND_MAX && max > min);
    return (rand() % (max - min)) + min;
  }

  inline float random (float min, float max)
  {
    assert(max >= min);
    return (float)rand() * ((max - min) / (float)RAND_MAX) + min;
  }

  template <class T> T ceilPow2 (T x)
  {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
  }

  inline int ceil (int i, int period)
  {
    assert(period > 0);
    return period * (i > 0) ? (1 + (i - 1) / period) : (i / period);
  }
};

