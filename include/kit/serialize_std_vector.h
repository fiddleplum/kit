#pragma once

#include "serialize.h"
#include <vector>

template <class T>
void serialize(std::ostream & out, std::vector<T> const & v, void(*serializeItem)(std::ostream &, T const &))
{
	serialize(out, (unsigned int)v.size());
	for(T const & item : v)
	{
		serializeItem(out, item);
	}
}

template <class T>
void deserialize(std::istream & in, std::vector<T> & v, void(*deserializeItem)(std::istream &, T &))
{
	unsigned int size;
	deserialize(in, size);
	v.clear();
	v.resize(size);
	for(T & item : v)
	{
		deserializeItem(in, item);
	}
}

