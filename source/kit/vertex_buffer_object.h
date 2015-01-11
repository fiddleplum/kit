#pragma once

#include <vector>

class VertexBufferObject
{
public:
	VertexBufferObject();

	~VertexBufferObject();

	void addVertexComponent(int location, unsigned int offset, unsigned int numDimensions);

	void clearVertexComponents();

	void setBytesPerVertex(unsigned int bytes);

	void setNumIndicesPerPrimitive(unsigned int num);

	void setVertices(void const * vertices, unsigned int numBytes, bool dynamic);

	void setIndices(unsigned int const * indices, unsigned int numIndices);

	void updateVertices(void const * vertices, unsigned int numBytes, unsigned int byteOffset);

	void render() const;

private:
	class VertexComponent
	{
	public:
		unsigned int index;
		unsigned int size;
		unsigned int offset;
	};
	unsigned int arrayBuffer;
	unsigned int elementArrayBuffer;
	unsigned int mode;
	unsigned int numIndices;
	unsigned int bytesPerVertex;
	std::vector<VertexComponent> vertexComponents;
};

