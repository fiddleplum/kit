#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <vector>
#include <string>

class VertexBufferObject
{
public:
	VertexBufferObject(Shader const & shader, Mesh const & mesh, bool dynamic);
	~VertexBufferObject();

	void render() const;

private:
	void update();
	bool dynamic;
	unsigned int num_indices;
	unsigned int stride;
	unsigned int id;
	unsigned int index_id;
};

