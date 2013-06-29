#pragma once

#include <vector>

class VertexBufferObject
{
public:
	enum class Type
	{
		Float,
		Int
	};

	VertexBufferObject();
	~VertexBufferObject();
	void mapLocationToVertexComponent(int location, unsigned int offset, Type type, unsigned int numDimensions);
	void setVertices(void const * vertices, unsigned int numVertices, unsigned int bytesPerVertex, bool dynamic);
	void updateVertices(void const * vertices, unsigned int numVertices, unsigned int vertexOffset);
	void setIndices(unsigned int const * indices, unsigned int numIndices, unsigned int numIndicesPerPrimitive);
	void render() const;

private:
	class VertexComponent
	{
	public:
		unsigned int index;
		unsigned int size;
		unsigned int type;
		unsigned int offset;
	};
	unsigned int mArrayBuffer;
	unsigned int mElementArrayBuffer;
	unsigned int mMode;
	unsigned int mNumIndices;
	unsigned int mBytesPerVertex;
	std::vector<VertexComponent> mVertexComponents;
};

