#pragma once

#include <vector>

class VertexBufferObject
{
public:
	class Config
	{
	public:
		class VertexComponent
		{
		public:
			enum Type
			{
				Float,
				Int
			};

			VertexComponent(int location, unsigned int offset, Type type, unsigned int numDimensions)
				: location(location), offset(offset), type(type), numDimensions(numDimensions) {}

			int location = 0;
			unsigned int offset = 0;
			Type type = Float;
			unsigned int numDimensions = 0;
		};

		// Vertex format
		void const * vertices = nullptr;
		unsigned int numVertices = 0;
		unsigned int bytesPerVertex = 0;
		bool dynamic = false;

		// Index format
		unsigned int const * indices = nullptr;
		unsigned int numIndices = 0;
		unsigned int numIndicesPerPrimitive = 0;
		
		std::vector<VertexComponent> vertexComponents;
	};

	VertexBufferObject(Config const & config);

	~VertexBufferObject();

	void updateVertices(void const * vertices, unsigned int numVertices, unsigned int vertexOffset);

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

