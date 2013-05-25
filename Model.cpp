#include "Model.h"
#include <fstream>
#include <stdexcept>

int readInt(std::istream & is)
{
	int v = 0;
	is.read((char *)&v, 4);
	if(!is.good())
	{
		throw std::exception();
	}
}

float readFloat(std::istream & is)
{
	float v = 0.0f;
	is.read((char *)&v, 4);
	if(!is.good())
	{
		throw std::exception();
	}
}

std::string readString(std::istream & is)
{
	std::string v;
	char c;
	do
	{
		is.read(&c, 1);
		if(!is.good())
		{
			throw std::exception();
		}
	} while(c != 0);
	return v;
}
Model::Model(std::string const & filename)
{
	std::fstream file(filename);
	try
	{
		int numMeshes = readInt(file);
		for(int meshI = 0; meshI < numMeshes; meshI++)
		{
			meshes.push_back(Mesh());
			Mesh & mesh = meshes.back();
			mesh.name = readString(file);
			mesh.material.diffuseColor[0] = readFloat(file);
			mesh.material.diffuseColor[1] = readFloat(file);
			mesh.material.diffuseColor[2] = readFloat(file);
			mesh.material.shininess = readInt(file);
			mesh.material.shininessStrength = readFloat(file);
			int numTextures = readInt(file);
			for(int textureI = 0; textureI < numTextures; textureI++)
			{
				mesh.material.textures.push_back(Texture());
				Texture & texture = mesh.material.textures.back();
				texture.filename = readString(file);
				texture.type = readString(file);
				texture.uvIndex = readInt(file);
			}
		}
	}
	catch (std::exception const &)
	{
		throw std::runtime_error("Error while loading " + filename + ". ");
	}
}
