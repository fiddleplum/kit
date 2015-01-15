#pragma once

#include "ptr.h"
#include "image.h"
#include "coord.h"
#include "matrix.h"
#include <string>
#include <vector>

class SceneModel
{
public:
	virtual void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs) = 0;

	virtual void setVertices(void const * vertices, unsigned int numBytes) = 0;

	virtual void setNumIndicesPerPrimitive(unsigned int num) = 0;

	virtual void setIndices(unsigned int const * indices, unsigned int numIndices) = 0;

	virtual Image getImageOfTexture(unsigned int textureIndex) const = 0;

	virtual void addTextureFromImage(std::string const & name, Image const & image, std::string const & type, unsigned int uvIndex) = 0;

	virtual void addTextureFromFile(std::string const & filename, std::string const & type, unsigned int uvIndex) = 0;

	virtual void clearTextures() = 0;

	virtual void setColor(Coord3f const & emitColor, Coord4f const & diffuseColor) = 0;

	virtual void setSpecular(unsigned int level, float strength) = 0;

	virtual float getScale() const = 0;

	virtual void setScale(float scale) = 0;
};

