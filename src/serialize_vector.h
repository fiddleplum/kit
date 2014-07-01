#pragma once

#include "serialize.h"
#include "vector.h"

namespace kit
{
	template <unsigned int dim, typename T>
	bool serialize(std::ostream & out, Vector<dim, T> const & v)
	{
		for(unsigned int i = 0; i < dim; i++)
		{
			if(!serialize(out, v[i]))
			{
				return false;
			}
		}
		return true;
	}

	template <unsigned int dim, typename T>
	bool deserialize(std::istream & in, Vector<dim, T> & v)
	{
		for(unsigned int i = 0; i < dim; i++)
		{
			if(!deserialize(in, v[i]))
			{
				return false;
			}
		}
		return true;
	}
}

