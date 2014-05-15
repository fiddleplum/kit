#pragma once

#include "vector.h"
#include "matrix.h"
#include <vector>
#include <istream>
#include <string>
#include <memory>
#include <set>

class VertexBufferObject;
class Texture;
class Shader;
class Camera2D;

// The Model2D serialized format is at the bottom.

class Model2D
{
public:
	class Vertex
	{
	public:
		Vector2f pos;
		Vector2f uv;
	};

	Model2D();
	~Model2D();

	int getZ() const { return z; }
	void setZ(int);

	Vector2f getScale() const { return scale; }
	void setScale(Vector2f);

	std::shared_ptr<Texture> getTexture() const { return texture; }
	void setTexture(std::string const & filename);

	Vector3f getColor() const { return color; }
	void setColor(Vector3f);

	void setVertices(void const * vertices, unsigned int numBytes);
	void setNumIndicesPerPrimitive(unsigned int num);
	void setIndices(unsigned int const * indices, unsigned int numIndices);

	void load(std::string const & filename);

	// For use with RenderEngine
	
	void render(std::shared_ptr<Camera2D const> camera, Matrix33f const & transform) const;

	bool isSorted() const;

	void setSorted();

	bool operator < (Model2D const & model) const;

	static const unsigned int maxLights = 4;

private:
	Model2D(Model2D const &);
	Model2D & operator =(Model2D const &);

	void updateShader();

	int z;
	Vector2f scale;

	std::shared_ptr<Texture> texture;
	Vector3f color;

	int numBytesPerVertex;
	VertexBufferObject * vertexBufferObject;

	std::shared_ptr<Shader> shader;
	bool sorted;

	int scaleLocation;
	int viewSizeLocation;
	int worldViewLocation;
	int samplerLocation;
	int colorLocation;
};

/*

Model2D File Format

strings are UTF-8 encoded. They have the number of bytes prepended as an unsigned int. They are not null-terminated.
bools is 8 bits.
ints, unsigned ints, and floats are 32 bits.
lists begin with an unsigned int indicating their length and then the elements tightly packed.

model:
string - texture filename
float[3] - color multiplier
vertex list - list of vertex components (vertices all contatenated, tightly packed)
unsigned int - number of indices per primitive
int list - list of indices that make up triangles

vertex format:
float[2] - position
float[2] - uv

*/

