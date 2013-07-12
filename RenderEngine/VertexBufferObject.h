#pragma once

#include <vector>

class VertexBufferObject
{
public:
	enum Type
	{
		Float,
		Int
	};

	VertexBufferObject();

	~VertexBufferObject();

	void addVertexComponent(int location, unsigned int offset, Type type, unsigned int numDimensions);

	void removeVertexComponent(int location);

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

