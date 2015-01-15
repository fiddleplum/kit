#include "resource_cache.h"
#include <fstream>

OwnPtr<ObjectCache<Texture>> textureCache;
OwnPtr<ObjectCache<Shader>> shaderCache;
OwnPtr<ObjectCache<SceneModel>> modelCache;

Ptr<Texture> ResourceCache::getTexture(std::string const & name, Coord2i size)
{
	return resourceCache->textureCache->get(name, size);
}

Ptr<Texture> ResourceCache::getTextureFromFile(std::string const & filename)
{
	return resourceCache->textureCache->get(filename, filename);
}

Ptr<Shader> ResourceCache::getShader(std::string const & name, std::string code[])
{
	return resourceCache->shaderCache->get(name, code);
}

Ptr<SceneModel> ResourceCache::getModel(std::string const & name)
{
	return resourceCache->modelCache->get(name);
}

Ptr<SceneModel> ResourceCache::getModelFromFile(std::string const & filename)
{
	return resourceCache->modelCache->get(filename);
}

ResourceCache::ResourceCache()
{
	shaderCache.create();
	textureCache.create();
	modelCache.create();
}

ResourceCache::~ResourceCache()
{
	modelCache.destroy();
	textureCache.destroy();
	shaderCache.destroy();
}

/*

Model File Format

strings are UTF-8 encoded. They have the number of bytes prepended as an unsigned int. They are not null-terminated.
bools is 8 bits.
ints, unsigned ints, and floats are 32 bits.

model:
material - the material
bool - has normal
bool - has tangent
bool - has color
unsigned int - num uvs
unsigned int - num vertex bytes
vertex list - list of vertex bytes
unsigned int - number of indices per primitive
unsigned int - num indices
int list - list of indices that make up triangles

material:
float[3] - emmisive color (0..1)
float[4] - diffuse color (0..1)
unsigned int - shininess (>= 1)
float - shininess strength (>= 0)
unsigned int - num textures
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
float[2] list - uvs

*/

