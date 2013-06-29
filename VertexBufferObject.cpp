#include "VertexBufferObject.h"
#include "OpenGL.h"
#include <stdexcept>

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &mArrayBuffer);
	glGenBuffers(1, &mElementArrayBuffer);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &mElementArrayBuffer);
	glDeleteBuffers(1, &mArrayBuffer);
}

void VertexBufferObject::mapLocationToVertexComponent(int location, unsigned int offset, Type type, unsigned int numDimensions)
{
	VertexComponent vertexComponent;
	vertexComponent.index = location;
	vertexComponent.size = numDimensions;
	switch(type)
	{
		case Type::Float:
			vertexComponent.type = GL_FLOAT; break;
		case Type::Int:
			vertexComponent.type = GL_INT; break;
	}
	vertexComponent.offset = offset;
	mVertexComponents.push_back(vertexComponent);
}

void VertexBufferObject::setVertices(void const * vertices, unsigned int numVertices, unsigned int bytesPerVertex, bool dynamic)
{
	mBytesPerVertex = bytesPerVertex;
	GLenum usage;
	if(dynamic)
	{
		usage = GL_DYNAMIC_DRAW;
	}
	else
	{
		usage = GL_STATIC_DRAW;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * bytesPerVertex, vertices, usage);
}

void VertexBufferObject::updateVertices(void const * vertices, unsigned int numVertices, unsigned int vertexOffset)
{
	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * mBytesPerVertex, numVertices * mBytesPerVertex, vertices);
}

void VertexBufferObject::setIndices(unsigned int const * indices, unsigned int numIndices, unsigned numIndicesPerPrimitive)
{
	mNumIndices = numIndices;

	switch(numIndicesPerPrimitive)
	{
		case 1:
			mMode = GL_POINTS; break;
		case 2:
			mMode = GL_LINES; break;
		case 3:
			mMode = GL_TRIANGLES; break;
		default:
			throw std::runtime_error("Wrong number of vertices per primitive. ");
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementArrayBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), (void const *)indices, GL_STATIC_DRAW);
}

void VertexBufferObject::render() const
{
	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	for(VertexComponent const & vertexComponent : mVertexComponents)
	{
		glEnableVertexAttribArray(vertexComponent.index); // DO I NEED THIS?
		glVertexAttribPointer(vertexComponent.index, vertexComponent.size, vertexComponent.type, GL_FALSE, mBytesPerVertex, (void const *)vertexComponent.offset);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementArrayBuffer);
	glDrawElements(mMode, mNumIndices, GL_UNSIGNED_INT, 0);
}

