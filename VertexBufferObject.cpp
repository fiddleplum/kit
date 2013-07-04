#include "VertexBufferObject.h"
#include "OpenGL.h"
#include <stdexcept>

VertexBufferObject::VertexBufferObject(Config const & config)
{
#ifdef DEBUG
	if(config.vertices == nullptr)
	{
		throw std::runtime_error("Vertices is null.");
	}
	if(config.bytesPerVertex == 0)
	{
		throw std::runtime_error("BytesPerVertex is zero.");
	}
	if(config.indices == nullptr)
	{
		throw std::runtime_error("Indices is null.");
	}
	if(config.numIndicesPerPrimitive < 1 || 3 < config.numIndicesPerPrimitive)
	{
		throw std::runtime_error("NumVerticesPerPrimitive is out of range.");
	}
	for(Config::VertexComponent const & vertexComponentConfig : config.vertexComponents)
	{
		if(vertexComponentConfig.numDimensions == 0)
		{
			throw std::runtime_error("VertexComponent.NumDimensions is zero.");
		}
	}
#endif

	// Setup array buffer
	glGenBuffers(1, &mArrayBuffer);
	mBytesPerVertex = config.bytesPerVertex;
	GLenum usage;
	if(config.dynamic)
	{
		usage = GL_DYNAMIC_DRAW;
	}
	else
	{
		usage = GL_STATIC_DRAW;
	}
	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, config.numVertices * config.bytesPerVertex, config.vertices, usage);

	// Setup element array buffer
	glGenBuffers(1, &mElementArrayBuffer);
	mNumIndices = config.numIndices;
	switch(config.numIndicesPerPrimitive)
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, config.numIndices * sizeof(unsigned int), (void const *)config.indices, GL_STATIC_DRAW);

	// Setup the vertex component attributes
	for(Config::VertexComponent const & vertexComponentConfig : config.vertexComponents)
	{
		VertexComponent vertexComponent;
		vertexComponent.index = vertexComponentConfig.location;
		vertexComponent.size = vertexComponentConfig.numDimensions;
		switch(vertexComponentConfig.type)
		{
			case Config::VertexComponent::Float:
				vertexComponent.type = GL_FLOAT; break;
			case Config::VertexComponent::Int:
				vertexComponent.type = GL_INT; break;
		}
		vertexComponent.offset = vertexComponentConfig.offset;
		mVertexComponents.push_back(vertexComponent);
	}
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &mElementArrayBuffer);
	glDeleteBuffers(1, &mArrayBuffer);
}

void VertexBufferObject::updateVertices(void const * vertices, unsigned int numVertices, unsigned int vertexOffset)
{
	glBindBuffer(GL_ARRAY_BUFFER, mArrayBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * mBytesPerVertex, numVertices * mBytesPerVertex, vertices);
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

