#pragma once

#include "serialize.h"
#include "coord.h"

template <unsigned int dim, typename T>
void serialize(std::ostream & out, Coord<dim, T> const & v)
{
	for(unsigned int i = 0; i < dim; i++)
	{
		serialize(out, v[i]);
	}
}

template <unsigned int dim, typename T>
void deserialize(std::istream & in, Coord<dim, T> & v)
{
	for(unsigned int i = 0; i < dim; i++)
	{
		deserialize(in, v[i]);
	}
}

