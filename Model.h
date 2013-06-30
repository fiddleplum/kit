#pragma once

#include <vector>
#include <istream>
#include <string>
#include "Vector.h"

// The model serialized format is at the bottom.

class Model
{
public:
	enum class TextureType
	{
		Diffuse,
		Normal,
		Reflection
	}

	class Material
	{
	public:
		Vector3f diffuseColor;
		int shininess;
		float shininessStrength;
		std::vector<Texture> textures;
	};

	Model();

	void addTexture(std::string const & filename, TextureType type, int uvIndex);

	void setColor(Vector3f color);

	void setSpecular(int level, float strength);

	void setVertices(void const * vertices, unsigned int numVertices, bool hasNormal, bool hasColor, unsigned int numUVs);

	void setIndices(unsigned int const * indices, unsigned int numIndices, unsigned int numIndicesPerPrimitive);

	Model(std::istream & in);

private:
	class Texture
	{
	public:
		std::string type;
		int uvIndex;
	};

	bool mVertexHasNormal;
	bool mVertexHasColor;
	unsigned int mNumVertexUVs;
	VertexBufferObject * mVertexBufferObject;
};

/*

Model File Format

Strings are UTF-8 encoded. They have the number of bytes prepended as an unsigned int. They are not null-terminated.
Bool is 8 bits.
Ints and floats are 32 bits.
Lists begin with an Int indicating their length and then the elements tightly packed.

model:
material - the material
bool - has normal
int - num uvs
float list - list of vertex components (vertices all contatenated, tightly packed)
int list - list of indices that make up triangles

material:
float[3] - diffuse color (0..1)
int - shininess (>= 1)
float - shininess strength (>= 0)
texture list - list of textures

texture:
string - filename
string - type
int - uv index

vertex format:
float[3] - position
float[3] - normal (if it has one)
float[2] list - uvs *** Note: this list is not prepended with a length.

*/

