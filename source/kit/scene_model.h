#pragma once

#include "texture.h"
#include "ptr.h"
#include "vector.h"
#include "matrix.h"
#include <string>
#include <vector>

class Shader;
class VertexBufferObject;

class SceneModel
{
public:
	// Create a blank model.
	SceneModel();

	// Load from a file.
	SceneModel(std::string const & filename);

	void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

	void setVertices(void const * vertices, unsigned int numBytes);

	void setNumIndicesPerPrimitive(unsigned int num);

	void setIndices(unsigned int const * indices, unsigned int numIndices);

	Ptr<Texture> getTexture(unsigned int textureIndex) const;

	void addTexture(Ptr<Texture> texture, std::string const & type, unsigned int uvIndex);

	void addTexture(std::string const & filename, std::string const & type, unsigned int uvIndex);

	void clearTextures();

	void setColor(Vector3f const & emitColor, Vector4f const & diffuseColor);

	void setSpecular(unsigned int level, float strength);

	float getScale() const;

	void setScale(float scale);

	void render(Matrix44f const & projectionTransform, Matrix44f const & localToCameraTransform, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

	static const unsigned int maxLights = 4;

	bool needsResorting() const;

	void resortingDone();

	bool operator < (SceneModel const & model) const;

private:
	class TextureInfo
	{
	public:
		Ptr<Texture> texture;
		std::string type;
		int samplerLocation;
		int uvIndex;
	};

	void updateShader();
	void updateShader120();
	void updateShader150();

	Vector3f emitColor;
	int emitColorLocation;
	Vector4f diffuseColor;
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

