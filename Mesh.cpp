#include "Mesh.h"
#include "SerializeStdVector.h"
#include "SeializeVector.h"

bool serialize(std::ostream & out, Mesh::VertexComponent const& v)
{
	return serialize(out, (unsigned int const &)v);
}

bool deserialize(std::istream & in, Mesh::VertexComponent & v)
{
	return deserialize(in, (unsigned int &)v);
}

bool Mesh::serialize(std::ostream & out) const
{
	if(!serialize(out, vertexFormat, serialize))
	{
		return false;
	}
	if(!serialize(out, data, serialize))
	{
		return false;
	}
	if(!serialize(out, numVerticesPerFrame))
	{
		return false;
	}
	if(!serialize(out, triangles, serialize))
	{
		return false;
	}
	return true;
}

bool Mesh::deserialize(std::istream & in)
{
	if(!deserialize(in, vertexFormat, deserialize))
	{
		return false;
	}
	if(!deserialize(in, data, deserialize))
	{
		return false;
	}
	if(!deserialize(in, numVerticesPerFrame))
	{
		return false;
	}
	if(!deserialize(in, triangles, deserialize))
	{
		return false;
	}
	return true;
}