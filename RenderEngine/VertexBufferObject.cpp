#include "VertexBufferObject.h"
#include "OpenGL.h"
#include <stdexcept>

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &mArrayBuffer);
	glGenBuffers(1, &mElementArrayBuffer);
	mMode = GL_TRIANGLES;
	mNumIndices = 0;
	mBytesPerVertex = 0;
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &mElementArrayBuffer);
	glDeleteBuffers(1, &mArrayBuffer);
}

void VertexBufferObject::addVertexComponent(int location, unsigned int offset, Type type, unsigned int numDimensions)
{
	VertexComponent vertexComponent;
	vertexComponent.index = location;
	vertexComponent.size = numDimensions;
	switch(type)
	{
		case Float:
			vertexComponent.type = GL_FLOAT; break;
		case Int:
			vertexComponent.type = GL_INT; break;
	}
	vertexComponent.offset = offset;
	mVertexComponents.push_back(vertexComponent);
}

void VertexBufferObject::clearVertexComponents()
{
	mVertexComponents.clear();
}

void VertexBufferObject::setBytesPerVertex(unsigned int bytes)
{
	mBytesPerVertex = bytes;
}

void VertexBufferObject::setNumIndicesPerPrimitive(unsigned int num)
{
	switch(num)
	{
	case 1:
		mMode = GL_POINTS; break;
	case 2:
		mMode = GL_LINES; break;
	case 3:
		mMode = GL_TRIANGLES; break;
	default:
		throw std::exception();
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
	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, numBytes, vertices, usage);
}

void VertexBufferObject::setIndices(unsigned int const * indices, unsigned int numIndices)
{
	mNumIndices = numIndices;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementArrayBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), (void const *)indices, GL_STATIC_DRAW);
}

void VertexBufferObject::updateVertices(void const * vertices, unsigned int numBytes, unsigned int byteOffset)
{
	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, byteOffset, numBytes, vertices);
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

