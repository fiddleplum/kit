#pragma once

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <ctime>
#ifndef _MSC_VER
#include <float.h>
#endif

namespace kit
{
	namespace math
	{
		extern bool firstRandom;

		const double PI = 3.1415926535897932384626433832795; // 180 degrees
		const double TWO_PI = 6.283185307179586476925286766559; // 360 degrees
		const double PI_OVER_2 = 1.5707963267948966192313216916398; // 90 degrees
		const double PI_OVER_3 = 1.0471975511965977461542144610932; // 60 degrees
		const double PI_OVER_4 = 0.78539816339744830961566084581988; // 45 degrees
		const double PI_OVER_6 = 0.52359877559829887307710723054658; // 30 degrees
		const double PI_OVER_12 = 0.26179938779914943653855361527329; // 15 degrees

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

		//! Returns true if v is not a number.
		template <class T> bool isNan (T v);

		//! Returns true if v is infinite.
		template <class T> bool isInf (T v);

		// Template Implementations.

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
			if(firstRandom)
			{
				std::srand((int)(std::time(nullptr)));
				firstRandom = false;
			}
			return (rand() % (max - min)) + min;
		}

		inline float random (float min, float max)
		{
			assert(max >= min);
			if(firstRandom)
			{
				std::srand((int)(std::time(nullptr)));
				firstRandom = false;
			}
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

		template <class T> bool isNan (T v)
		{
	#ifndef _MSC_VER
			return std::isnan(v);
	#else
			return _isnan(v);
	#endif
		}

		template <class T> bool isInf (T v)
		{
	#ifndef _MSC_VER
			return std::isinf(v);
	#else
			return _isinf(v);
	#endif
		}
	}
}

