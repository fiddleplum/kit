#pragma once

#include <istream>
#include <ostream>
#include <cstdint>

// In each of these functions, the streams should be set to read or write in
// binary mode. Make sure that T contains no pointers or other complex objects.
// Returns true on success. For error specifics, check the stream status.

template<class T>
bool serialize (std::ostream & out, T const& t)
{
	out.write((char const*) &t, sizeof(T));
	return out.good();
}

template<class T>
bool deserialize (std::istream & in, T & t)
{
	in.read((char *) &t, sizeof(T));
	return in.good();
}

