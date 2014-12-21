#pragma once

#include "Vector.h"
#include <vector>
#include <ostream>
#include <iostream>
#include <cstdint>

class Mesh
{
public:
	class Texture
	{
	public:
		std::string filename;
		std::string type;
		int uvIndex;
	};

	class Material
	{
	public:
		Vector3f diffuseColor;
		int shininess;
		float shininessStrength;
		std::vector<Texture> textures;
	};

	Material material;
	int numUVs;
	std::vector<float> vertices;
	std::vector<int> indices;
};

bool serialize(std::ostream & out, Mesh const & mesh);
bool deserialize(std::istream & in, Mesh & mesh);

