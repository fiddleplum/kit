#pragma once

#include <function>
#include "Serialize.h"

template <class T>
bool serialize(std::ostream & out, std::vector<T> const & v, std::function<bool(std::ostream &, T const &)> serializeItem)
{
	if(!serialize(out, (unsigned int)v.size()))
	{
		return false;
	}
	for(T const & item : v)
	{
		if(!serializeItem(out, item))
		{
			return false;
		}
	}
	return true;
}

template <class T>
bool deserialize(std::istream & in, std::vector<T> & v, std::function<bool(std::istream &, T &)> deserializeItem)
{
	unsigned int size;
	if(!deserialize(in, size))
	{
		return false;
	}
	v.clear();
	v.resize(size);
	for(T & item : v)
	{
		if(!deserializeItem(in, item))
		{
			return false;
		}
	}
	return true;
}

