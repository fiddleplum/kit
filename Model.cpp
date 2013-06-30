#include "Model.h"
#include "Serialize.h"
#include "SerializeStdString.h"
#include "SerializeStdVector.h"
#include "Texture.h"
#include "ResourceManager.h"
#include <istream>



Model::Model()
{
	mVertexHasNormal = false;
	mVertexHasColor = false;
	mNumVertexUVs = 0;
	mVertexBufferObject = nullptr;
}

Model::Model(std::istream & in)
{
	deserialize(in, material.diffuseColor[0]);
	deserialize(in, material.diffuseColor[1]);
	deserialize(in, material.diffuseColor[2]);
	deserialize(in, material.shininess);
	deserialize(in, material.shininessStrength);
	deserialize<Texture>(in, material.textures, [] (std::istream & in, Texture & texture)
	{
		deserialize(in, texture.filename);
		deserialize(in, texture.type);
		deserialize(in, texture.uvIndex);
	});
	deserialize(in, hasNormal);
	deserialize(in, numUVs);
	int numVertices = 0;
	deserialize(in, numVertices);
	int numFloats = 3 + 3 + 2 * numUVs;
	vertices.resize(numFloats);
	for(int vertexI = 0; vertexI < numFloats; vertexI++)
	{
		deserialize(in, vertices[vertexI]);
	}
	deserialize<int>(in, indices, deserialize);
}

void Model::addTexture(std::string const & filename, TextureType type, int uvIndex)
{
	std::shared_ptr<Texture> texture = gTextureManager->get(filename);
}

