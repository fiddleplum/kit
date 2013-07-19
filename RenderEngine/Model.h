#pragma once

#include "../Vector.h"
#include "Frame.h"
#include <vector>
#include <istream>
#include <string>
#include <memory>

class Scene;
class Camera;
class VertexBufferObject;
class Texture;
class Shader;

// The model serialized format is at the bottom.

class Model
{
public:
	Model(Scene * scene);

	~Model();

	void load(std::string const & filename);

	void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

	void setVertices(void const * vertices, unsigned int numBytes);

	void setNumIndicesPerPrimitive(unsigned int num);

	void setIndices(unsigned int const * indices, unsigned int numIndices);

	void addTexture(std::string const & filename, std::string const & type, unsigned int uvIndex);

	void clearTextures();

	void setColor(Vector4f const & color);

	void setSpecular(unsigned int level, float strength);

	Frame const & getFrame() const;

	Frame & getFrame();

	Model(Model const &) = delete;

	Model & operator =(Model const &) = delete;

	// For use with RenderEngine
	
	void render(Camera const * camera) const;

	bool needsResorting() const;

	void resortingDone();

	bool operator < (Model const & model) const;

private:
	class TextureInfo
	{
	public:
		std::shared_ptr<Texture> texture;
		std::string type;
		int samplerLocation;
		int uvIndex;
	};

	void updateShader();

	Scene * mScene;

	Vector4f mColor;
	int mColorLocation;
	unsigned int mSpecularLevel;
	int mSpecularLevelLocation;
	float mSpecularStrength;
	int mSpecularStrengthLocation;

	std::vector<TextureInfo> mTextureInfos;

	bool mVertexHasNormal;
	bool mVertexHasTangent;
	bool mVertexHasColor;
	unsigned int mNumVertexUVs;
	unsigned int mNumBytesPerVertex;
	VertexBufferObject * mVertexBufferObject;

	std::shared_ptr<Shader> mShader;
	bool mNeedsResorting;

	Frame mFrame;
	int mProjectionLocation;
	int mWorldViewLocation;
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

