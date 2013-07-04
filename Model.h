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

			Texture(std::string const & filename, Type type, int uvIndex)
				: filename(filename), type(type), uvIndex(uvIndex) {}

			std::string filename;
			Type type = Diffuse;
			int uvIndex = -1; // -1 means no uv.
		};

		// Vertex format
		void const * vertices = nullptr;
		unsigned int numVertices = 0;
		bool vertexHasNormal = false;
		bool vertexHasTangent = false;
		bool vertexHasColor = false;
		unsigned int numVertexUVs = 0;

		// Index format
		unsigned int const * indices = nullptr;
		unsigned int numIndices = 0;
		unsigned int numIndicesPerPrimitive = 0;

		// Material
		bool hasBaseColor = false;
		Vector4f baseColor = Vector4f::zero();
		std::vector<Texture> textures;
		unsigned int specularLevel = 0;
		float specularStrength = 0.0f;
	};

	Model(Config const & config);

	// Model(std::istream & in);

	~Model();

	void render() const;

private:
	class TextureInfo
	{
	public:
		std::shared_ptr<Texture> texture;
		int samplerLocation;
		int uvIndex;
	};

	void setupShader(Config const & config);

	std::shared_ptr<Shader> mShader;
	std::vector<TextureInfo> mTextureInfos;
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

