#pragma once

#include "Vector.h"
#include <vector>
#include <ostream>
#include <iostream>
#include <cstdint>

class Mesh
{
public:
	enum VertexComponent
	{
		Position,
		Normal,
		TexCoord
	};

	bool serialize(std::ostream & out) const;
	bool deserialize(std::istream & in);

	std::vector<VertexComponent> vertexFormat; ///< Describes the components in each vertex.
	std::vector<float> data; ///< Vertex data is interleaved, and contains one or more frames of data in sequence.
    std::vector<Vector3i> triangles; ///< Each vertex points to a vertex in data.
    unsigned int numVerticesPerFrame; ///< Each from contains the same number of vertices.
};

