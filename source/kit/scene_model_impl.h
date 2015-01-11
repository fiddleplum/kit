#pragma once

#include <kit/scene_model.h>
#include <string>
#include <vector>

class Texture;
class Shader;
class VertexBufferObject;

class SceneModelImpl : public virtual SceneModel
{
public:
	SceneModelImpl();

	SceneModelImpl(std::string const & filename);

	void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs) override;

	void setVertices(void const * vertices, unsigned int numBytes) override;

	void setNumIndicesPerPrimitive(unsigned int num) override;

	void setIndices(unsigned int const * indices, unsigned int numIndices) override;

	Image getImageOfTexture(unsigned int textureIndex) const override;

	void addTextureFromImage(std::string const & name, Image const & image, std::string const & type, unsigned int uvIndex) override;

	void addTextureFromFile(std::string const & filename, std::string const & type, unsigned int uvIndex) override;

	void clearTextures() override;

	void setColor(Vector3f const & emitColor, Vector4f const & diffuseColor) override;

	void setSpecular(unsigned int level, float strength) override;

	float getScale() const override;

	void setScale(float scale) override;

	void render(Matrix44f const & projectionTransform, Matrix44f const & localToCameraTransform, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

	static const unsigned int maxLights = 4;

	bool needsResorting() const;

	void resortingDone();

	bool operator < (SceneModelImpl const & model) const;

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

