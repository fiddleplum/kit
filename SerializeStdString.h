#pragma once

#include "Serialize.h"

inline bool serialize(std::ostream & out, std::string const & v);
{
	if(!serialize(out, (unsigned int)v.size()))
	{
		return false;
	}
	if(!serialize(out, (void const *)&v[0], v.size()))
	{
		return false;
	}
	return true;
}

inline bool deserialize(std::istream & in, std::string & v)
{
	unsigned int size;
	if(!deserialize(in, size))
	{
		return false;
	}
	v.clear();
	v.resize(size);
	if(!deserialize(in, (void *)&v[0], v.size()))
	{
		return false;
	}
	return true;
}

