#pragma once

#include "ptr.h"
#include "coord.h"
#include "matrix.h"
#include "shader.h"
#include "vertex_buffer_object.h"
#include "texture.h"
#include <string>
#include <vector>

class SceneModel
{
public:
	SceneModel();

	SceneModel(std::string const & filename);

	void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

	void setVertices(void const * vertices, unsigned int numBytes);

	void setNumIndicesPerPrimitive(unsigned int num);

	void setIndices(unsigned int const * indices, unsigned int numIndices);

	void addTextureFromFile(std::string const & filename, std::string const & type, unsigned int uvIndex);

	void clearTextures();

	void setColor(Coord3f const & emitColor, Coord4f const & diffuseColor);

	void setSpecular(unsigned int level, float strength);

	float getScale() const;

	void setScale(float scale);

	void render(Matrix44f const & projectionTransform, Matrix44f const & localToCameraTransform, std::vector<Coord3f> const & lightPositions, std::vector<Coord3f> const & lightColors) const;

	bool needsResorting() const;

	void resortingDone();

	bool operator < (SceneModel const & model) const;

	static const unsigned int maxLights = 4;

private:
	struct TextureInfo
	{
		Ptr<Texture> texture;
		std::string type;
		int samplerLocation;
		int uvIndex;
	};

	void updateShader();
	void updateShader120();
	void updateShader150();

	Coord3f emitColor;
	int emitColorLocation;
	Coord4f diffuseColor;
	int diffuseColorLocation;
	unsigned int specularLevel;
	int specularLevelLocation;
	float specularStrength;
	int specularStrengthLocation;

	int lightPositionsLocation;
	int lightColorsLocation;

	std::vector<TextureInfo> textureInfos;

	bool vertexHasNormal;
	bool vertexHasTangent;
	bool vertexHasColor;
	unsigned int numVertexUVs;
	unsigned int numBytesPerVertex;
	OwnPtr<VertexBufferObject> vertexBufferObject;

	Ptr<Shader> shader;
	bool shaderDirty;

	bool sorted;

	int projectionLocation;
	int worldViewLocation;

	float scale;
	int scaleLocation;
};

