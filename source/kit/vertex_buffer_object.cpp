#include "vertex_buffer_object.h"
#include "open_gl.h"

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &arrayBuffer);
	glGenBuffers(1, &elementArrayBuffer);
	mode = GL_TRIANGLES;
	numIndices = 0;
	bytesPerVertex = 0;
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &elementArrayBuffer);
	glDeleteBuffers(1, &arrayBuffer);
}

void VertexBufferObject::addVertexComponent(int location, unsigned int offset, unsigned int numDimensions)
{
	VertexComponent vertexComponent;
	vertexComponent.index = location;
	vertexComponent.size = numDimensions;
	vertexComponent.offset = offset;
	vertexComponents.push_back(vertexComponent);
}

void VertexBufferObject::clearVertexComponents()
{
	vertexComponents.clear();
}

void VertexBufferObject::setBytesPerVertex(unsigned int bytes)
{
	bytesPerVertex = bytes;
}

void VertexBufferObject::setNumIndicesPerPrimitive(unsigned int num)
{
	switch(num)
	{
		case 1:
			mode = GL_POINTS; break;
		case 2:
			mode = GL_LINES; break;
		case 3:
		default:
			mode = GL_TRIANGLES; break;
	}
}

void VertexBufferObject::setVertices(void const * vertices, unsigned int numBytes, bool dynamic)
{
	GLenum usage;
	if(dynamic)
	{
		usage = GL_DYNAMIC_DRAW;
	}
	else
	{
		usage = GL_STATIC_DRAW;
	}
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, numBytes, vertices, usage);
}

void VertexBufferObject::setIndices(unsigned int const * indices, unsigned int numIndices_)
{
	numIndices = numIndices_;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices_ * sizeof(unsigned int), (void const *)indices, GL_STATIC_DRAW);
}

void VertexBufferObject::updateVertices(void const * vertices, unsigned int numBytes, unsigned int byteOffset)
{
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, byteOffset, numBytes, vertices);
}

void VertexBufferObject::render() const
{
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	for(VertexComponent const & vertexComponent : vertexComponents)
	{
		glEnableVertexAttribArray(vertexComponent.index);
		glVertexAttribPointer(vertexComponent.index, vertexComponent.size, GL_FLOAT, GL_FALSE, bytesPerVertex, (void const *)vertexComponent.offset);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
	glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0);
}

