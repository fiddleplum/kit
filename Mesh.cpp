#include "Mesh.h"
#include "SerializeStdVector.h"
#include "SerializeVector.h"

bool serialize(std::ostream & out, Mesh::VertexComponent const & v)
{
	return serialize(out, (unsigned int const &)v);
}

bool deserialize(std::istream & in, Mesh::VertexComponent & v)
{
	return deserialize(in, (unsigned int &)v);
}

bool serialize(std::ostream & out, Mesh const & mesh)
{
	if(!serialize(out, mesh.vertexFormat, serialize))
	{
		return false;
	}
	if(!serialize(out, mesh.data, serialize))
	{
		return false;
	}
	if(!serialize(out, mesh.numVerticesPerFrame))
	{
		return false;
	}
	if(!serialize(out, mesh.triangles, serialize<3, int>))
	{
		return false;
	}
	return true;
}

bool deserialize(std::istream & in, Mesh & mesh)
{
	if(!deserialize(in, mesh.vertexFormat, deserialize))
	{
		return false;
	}
	if(!deserialize(in, mesh.data, deserialize))
	{
		return false;
	}
	if(!deserialize(in, mesh.numVerticesPerFrame))
	{
		return false;
	}
	if(!deserialize(in, mesh.triangles, deserialize<3, int>))
	{
		return false;
	}
	return true;
}