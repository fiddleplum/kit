#pragma once

#include "Vector.h"
#include <vector>
#include <istream>
#include <string>
#include <memory>

class VertexBufferObject;
class Texture;
class Shader;

// The model serialized format is at the bottom.

class Model
{
public:
	class Config
	{
	public:
		class Texture
		{
		public:
			enum Type
			{
				Diffuse,
				Normal,
				Reflection
			};

			Texture() {}
			Texture(std::string const & filename, Type type, int uvIndex)
				: filename(filename), type(type), uvIndex(uvIndex) {}

			std::string filename;
			Type type = Diffuse;
			int uvIndex = -1; // -1 means no uv.
		};

		// Material
		Vector4f baseColor = Vector4f::zero();
		std::vector<Texture> textures;
		unsigned int specularLevel = 0;
		float specularStrength = 0.0f;
	};

	Model(Config const & config);

	Model(std::istream & in);

	~Model();

	std::shared_ptr<Shader const> getShader() const;

	void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

	void setVertices(void const * vertices, unsigned int numBytes);

	void setNumIndicesPerPrimitive(unsigned int num);

	void setIndices(unsigned int const * indices, unsigned int numIndices);

	void setBaseColor(bool hasBaseColor, 

	void render() const;

	Model(Model const &) = delete;

	Model & operator =(Model const &) = delete;

private:
	class TextureInfo
	{
	public:
		std::shared_ptr<Texture> texture;
		int samplerLocation;
		int uvIndex;
	};

	void construct(Config const & config);
	void setupShader(Config const & config);

	bool mShaderNeedsUpdate;
	bool mVertexHasNormal;
	bool mVertexHasTangent;
	bool mVertexHasColor;
	unsigned int mNumVertexUVs;
	Vector4f mColor;
	std::shared_ptr<Shader> mShader;
	std::vector<TextureInfo> mTextureInfos;
	VertexBufferObject * mVertexBufferObject;
};

/*

Model File Format

strings are UTF-8 encoded. They have the number of bytes prepended as an unsigned int. They are not null-terminated.
bools is 8 bits.
ints, unsigned ints, and floats are 32 bits.
lists begin with an unsigned int indicating their length and then the elements tightly packed.

model:
material - the material
bool - has normal
bool - has tangent
bool - has color
unsigned int - num uvs
vertex list - list of vertex components (vertices all contatenated, tightly packed)
unsigned int - number of indices per primitive
int list - list of indices that make up triangles

material:
float[4] - color (0..1)
unsigned int - shininess (>= 1)
float - shininess strength (>= 0)
texture list - list of textures

texture:
string - filename
string - type
int - uv index

vertex format:
float[3] - position
float[3] - normal (if it has one)
float[3] - tangent (if it has one)
float[4] - color (if it has one)
float[2] list - uvs *** Note: this list is not prepended with a length.

*/

