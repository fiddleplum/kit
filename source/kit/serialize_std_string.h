#pragma once

#include "serialize.h"

inline void serialize(std::ostream & out, std::string const & v)
{
	serialize(out, (unsigned int)v.size());
	serialize(out, (void const *)&v[0], v.size());
}

inline void deserialize(std::istream & in, std::string & v)
{
	unsigned int size;
	deserialize(in, size);
	v.clear();
	v.resize(size);
	deserialize(in, (void *)&v[0], v.size());
}

