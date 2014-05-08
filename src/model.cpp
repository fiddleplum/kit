#include "model.h"
#include "camera.h"
#include "app.h"
#include "shader.h"
#include "texture.h"
#include "vertex_buffer_object.h"
#include "serialize.h"
#include "serialize_std_string.h"
#include "serialize_std_vector.h"
#include "string_util.h"
#include <fstream>
#include <stdexcept>

Model::Model()
{
	mVertexHasNormal = false;
	mVertexHasTangent = false;
	mVertexHasColor = false;
	mNumVertexUVs = 0;
	mEmitColor = Vector3f::zero();
	mDiffuseColor = Vector4f(1, 1, 1, 1);
	mSpecularLevel = 1;
	mSpecularStrength = 0;
	mVertexBufferObject = new VertexBufferObject();
	mVertexBufferObject->setBytesPerVertex(sizeof(Vector3f));
	updateShader();
	mNeedsResorting = true;
}

Model::~Model()
{
	delete mVertexBufferObject;
}

void Model::load(std::string const & filename)
{
	std::fstream in (filename, std::fstream::in | std::fstream::binary);

	// Material
	Vector3f emitColor;
	deserialize(in, emitColor[0]);
	deserialize(in, emitColor[1]);
	deserialize(in, emitColor[2]);
	Vector4f diffuseColor;
	deserialize(in, diffuseColor[0]);
	deserialize(in, diffuseColor[1]);
	deserialize(in, diffuseColor[2]);
	deserialize(in, diffuseColor[3]);
	setColor(emitColor, diffuseColor);
	unsigned int specularLevel;
	float specularStrength;
	deserialize(in, specularLevel);
	deserialize(in, specularStrength);
	setSpecular(specularLevel, specularStrength);

	// Textures
	clearTextures();
	unsigned int numTextures;
	deserialize(in, numTextures);
	for(unsigned int i = 0; i < numTextures; i++)
	{
		std::string filename;
		deserialize(in, filename);
		std::string type;
		deserialize(in, type);
		unsigned int uvIndex;
		deserialize(in, uvIndex);
		addTexture(filename, type, uvIndex);
	}

	// Vertex format
	bool vertexHasNormal, vertexHasTangent, vertexHasColor;
	unsigned int numVertexUVs;
	deserialize(in, vertexHasNormal);
	deserialize(in, vertexHasTangent);
	deserialize(in, vertexHasColor);
	deserialize(in, numVertexUVs);
	setVertexFormat(vertexHasNormal, vertexHasTangent, vertexHasColor, numVertexUVs);

	// Vertices
	unsigned int numVertices;
	deserialize(in, numVertices);
	std::vector<unsigned char> vertices;
	vertices.resize(numVertices * mNumBytesPerVertex);
	deserialize(in, (void *)&vertices[0], numVertices * mNumBytesPerVertex);
	setVertices((void const *)&vertices[0], numVertices * mNumBytesPerVertex);

	// Index format
	unsigned int numIndicesPerPrimitive;
	deserialize(in, numIndicesPerPrimitive);
	setNumIndicesPerPrimitive(numIndicesPerPrimitive);

	// Indices
	std::vector<unsigned int> indices;
	deserialize<unsigned int>(in, indices, deserialize);
	setIndices(&indices[0], indices.size());
}

void Model::setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs)
{
	mNumBytesPerVertex = sizeof(Vector3f); // position
	mVertexHasNormal = hasNormal;
	if(mVertexHasNormal)
	{
		mNumBytesPerVertex += sizeof(Vector3f);
	}
	mVertexHasTangent = hasTangent;
	if(mVertexHasTangent)
	{
		mNumBytesPerVertex += sizeof(Vector3f);
	}
	mVertexHasColor = hasColor;
	if(mVertexHasColor)
	{
		mNumBytesPerVertex += sizeof(Vector4f);
	}
	mNumVertexUVs = numVertexUVs;
	mNumBytesPerVertex += numVertexUVs * 2 * sizeof(Vector2f);
	mVertexBufferObject->setBytesPerVertex(mNumBytesPerVertex);
	updateShader();
}

void Model::setVertices(void const * vertices, unsigned int numBytes)
{
	mVertexBufferObject->setVertices(vertices, numBytes, false);
}

void Model::setNumIndicesPerPrimitive(unsigned int num)
{
	mVertexBufferObject->setNumIndicesPerPrimitive(num);
}

void Model::setIndices(unsigned int const * indices, unsigned int numIndices)
{
	mVertexBufferObject->setIndices(indices, numIndices);
}

std::shared_ptr<Texture> Model::getTexture(unsigned int textureIndex) const
{
	if(textureIndex < mTextureInfos.size())
	{
		return mTextureInfos[textureIndex].texture;
	}
	else
	{
		return std::shared_ptr<Texture>();
	}
}

void Model::addTexture(std::string const & filename, std::string const & type, unsigned int uvIndex)
{
	TextureInfo textureInfo;
	textureInfo.texture = App::getTextureManager()->get(filename, filename);
	textureInfo.type = type;
	textureInfo.samplerLocation = -1;
	textureInfo.uvIndex = uvIndex;
	mTextureInfos.push_back(textureInfo);
	updateShader();
}

void Model::clearTextures()
{
	mTextureInfos.clear();
	updateShader();
}

void Model::setColor(Vector3f const & emitColor, Vector4f const & diffuseColor)
{
	mEmitColor = emitColor;
	mDiffuseColor = diffuseColor;
}

void Model::setSpecular(unsigned int level, float strength)
{
	mSpecularLevel = level;
	mSpecularStrength = strength;
}

void Model::render(std::shared_ptr<Camera const> camera, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const
{
	// The render engine handles shader and texture activation.
	mShader->activate();
	mShader->setUniform(mProjectionLocation, camera->getProjection());
	mShader->setUniform(mWorldViewLocation, camera->getView() * getMatrix());
	unsigned int samplerIndex = 0;
	for(unsigned int i = 0; i < mTextureInfos.size(); i++)
	{
		mTextureInfos[i].texture->activate(i);
		mShader->setUniform(mTextureInfos[i].samplerLocation, (int)samplerIndex);
		samplerIndex++;
	}
	Texture::deactivateRest(mTextureInfos.size());
	if(lightPositions.size() > 0)
	{
		mShader->setUniform(mLightPositionsLocation, &lightPositions[0], lightPositions.size());
		mShader->setUniform(mLightColorsLocation, &lightColors[0], lightColors.size());
	}
	mShader->setUniform(mEmitColorLocation, mEmitColor);
	mShader->setUniform(mDiffuseColorLocation, mDiffuseColor);
	mShader->setUniform(mSpecularLevelLocation, (int)mSpecularLevel);
	mShader->setUniform(mSpecularStrengthLocation, mSpecularStrength);
	mVertexBufferObject->render();
}

bool Model::needsResorting() const
{
	return mNeedsResorting;
}

void Model::resortingDone()
{
	mNeedsResorting = false;
}

bool Model::operator <(Model const & model) const
{
	if(mShader < model.mShader)
	{
		return true;
	}
	else if(mShader > model.mShader)
	{
		return false;
	}
	for(unsigned int i = 0; i < std::min(mTextureInfos.size(), model.mTextureInfos.size()); i++)
	{
		if(mTextureInfos[i].texture < model.mTextureInfos[i].texture)
		{
			return true;
		}
		else if(mTextureInfos[i].texture > model.mTextureInfos[i].texture)
		{
			return false;
		}
	}
	if(mTextureInfos.size() < model.mTextureInfos.size())
	{
		return true;
	}
	else if(mTextureInfos.size() > model.mTextureInfos.size())
	{
		return false;
	}
	return mVertexBufferObject < model.mVertexBufferObject;
}

void Model::updateShader()
{
	std::string code [Shader::NumCodeTypes];

	std::vector<std::string> uvIndexStrings;
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		uvIndexStrings.push_back(std::to_string(uvIndex));
	}

	/** VERTEX **/
	code[Shader::Vertex] += "#version 150\n";

	// Add the global variables.
	code[Shader::Vertex] += "uniform mat4 uWorldView;\n";
	code[Shader::Vertex] += "uniform mat4 uProjection;\n";
	code[Shader::Vertex] += "in vec3 aPosition;\n";
	code[Shader::Vertex] += "out vec3 vPosition;\n";
	if(mVertexHasNormal)
	{
		code[Shader::Vertex] += "in vec3 aNormal;\n";
		code[Shader::Vertex] += "out vec3 vNormal;\n";
	}
	if(mVertexHasTangent)
	{
		code[Shader::Vertex] += "in vec3 aTangent;\n";
		code[Shader::Vertex] += "out vec3 vTangent;\n";
	}
	if(mVertexHasColor)
	{
		code[Shader::Vertex] += "in vec4 aColor;\n";
		code[Shader::Vertex] += "out vec4 vColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		code[Shader::Vertex] += "in vec2 aUV" + uvIndexStrings[uvIndex] + ";\n";
		code[Shader::Vertex] += "out vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
	}

	// Add the main function.
	code[Shader::Vertex] += "void main()\n";
	code[Shader::Vertex] += "{\n";
	code[Shader::Vertex] += "	gl_Position = uProjection * uWorldView * vec4(aPosition, 1);\n";
	code[Shader::Vertex] += "	vPosition = (uWorldView * vec4(aPosition, 1)).xyz;\n";
	if(mVertexHasNormal)
	{
		code[Shader::Vertex] += "	vNormal = (uWorldView * vec4(aNormal, 0)).xyz;\n";
	}
	if(mVertexHasTangent)
	{
		code[Shader::Vertex] += "	vTangent = (uWorldView * vec4(aTangent, 0)).xyz;\n";
	}
	if(mVertexHasColor)
	{
		code[Shader::Vertex] += "	vColor = aColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		code[Shader::Vertex] += "	vUV" + uvIndexStrings[uvIndex] + " = aUV" + uvIndexStrings[uvIndex] + ";\n";
	}
	code[Shader::Vertex] += "}\n";

	/** FRAGMENT **/
	code[Shader::Fragment] += "#version 150\n";

	// Add the global variables.
	code[Shader::Fragment] += "in vec3 vPosition;\n";
	code[Shader::Fragment] += "uniform vec3 uEmitColor;\n";
	if(mVertexHasNormal)
	{
		code[Shader::Fragment] += "uniform vec3 uLightPositions [" + std::to_string(maxLights) + "];\n";
		code[Shader::Fragment] += "uniform vec3 uLightColors [" + std::to_string(maxLights) + "];\n";
		code[Shader::Fragment] += "in vec3 vNormal;\n";
		if(mVertexHasTangent)
		{
			code[Shader::Fragment] += "in vec3 vTangent;\n";
		}
	}
	if(mVertexHasColor)
	{
		code[Shader::Fragment] += "in vec4 vColor;\n";
	}
	else
	{
		code[Shader::Fragment] += "uniform vec4 uDiffuseColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		code[Shader::Fragment] += "in vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
	}
	unsigned int samplerIndex = 0;
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		code[Shader::Fragment] += "uniform sampler2D uSampler" + std::to_string(samplerIndex) + ";\n";
		samplerIndex++;
	}

	// Add the main function.
	code[Shader::Fragment] += "void main()\n";
	code[Shader::Fragment] += "{\n";
	if(mVertexHasColor)
	{
		code[Shader::Fragment] += "	vec4 color = vColor;\n";
	}
	else
	{
		code[Shader::Fragment] += "	vec4 color = uDiffuseColor;\n";
	}
	samplerIndex = 0;
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		std::string samplerIndexString = std::to_string(samplerIndex);
		if(textureInfo.type == "diffuse")
		{
			code[Shader::Fragment] += "	vec4 textureColor" + samplerIndexString + " = texture2D(uSampler" + samplerIndexString + ", vUV" + std::to_string(textureInfo.uvIndex) + ");\n";
			code[Shader::Fragment] += "	color = (1.0f - textureColor" + samplerIndexString + ".w) * color + textureColor" + samplerIndexString + ".w * textureColor" + samplerIndexString + ";\n";
		}
		else if(textureInfo.type == "normal")
		{
		}
		else if(textureInfo.type == "reflection")
		{
		}
		samplerIndex++;
	}
	if(mVertexHasNormal)
	{
		code[Shader::Fragment] += "	gl_FragColor = vec4(0, 0, 0, color.a);\n";
		code[Shader::Fragment] += "	for(int i = 0; i < " + std::to_string(maxLights) + "; i++)\n";
		code[Shader::Fragment] += "	{\n";
		code[Shader::Fragment] += "		float dotLight = dot(normalize(uLightPositions[i] - vPosition), vNormal);\n";
		code[Shader::Fragment] += "		if(dotLight > 0)\n";
		code[Shader::Fragment] += "		{\n";
		code[Shader::Fragment] += "			gl_FragColor.rgb += color.rgb * uLightColors[i] * dotLight;\n";
		code[Shader::Fragment] += "		}\n";
		code[Shader::Fragment] += "	}\n";
	}
	else
	{
		code[Shader::Fragment] += "	gl_FragColor = color;\n";
	}
	code[Shader::Fragment] += "	gl_FragColor.rgb += uEmitColor;\n";
	code[Shader::Fragment] += "}\n";

	// Create a unique name for the shader.
	std::string name;
	name = "Model";
	if(mVertexHasNormal)
	{
		name += "n";
	}
	if(mVertexHasTangent)
	{
		name += "t";
	}
	if(mVertexHasColor)
	{
		name += "c";
	}
	name += std::to_string(mNumVertexUVs);
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		name += textureInfo.type[0] + std::to_string(textureInfo.uvIndex);
	}

	mShader = App::getShaderManager()->get(name, code);
	mNeedsResorting = true;

	// Update attribute locations
	mVertexBufferObject->clearVertexComponents();
	unsigned int offset = 0;
	mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aPosition"), offset, 3);
	offset += sizeof(Vector3f);
	if(mVertexHasNormal)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aNormal"), offset, 3);
		offset += sizeof(Vector3f);
	}
	if(mVertexHasTangent)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aTangent"), offset, 3);
		offset += sizeof(Vector3f);
	}
	if(mVertexHasColor)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aColor"), offset, 4);
		offset += sizeof(Vector4f);
	}
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aUV" + std::to_string(textureInfo.uvIndex)), offset + textureInfo.uvIndex * sizeof(Vector2f), 2);
	}

	// Update uniform locations
	mLightPositionsLocation = mShader->getUniformLocation("uLightPositions");
	mLightColorsLocation = mShader->getUniformLocation("uLightColors");
	mEmitColorLocation = mShader->getUniformLocation("uEmitColor");
	mDiffuseColorLocation = mShader->getUniformLocation("uDiffuseColor");
	mSpecularLevelLocation = mShader->getUniformLocation("uSpecularLevel");
	mSpecularStrengthLocation = mShader->getUniformLocation("uSpecularStrength");
	for(TextureInfo & textureInfo : mTextureInfos)
	{
		textureInfo.samplerLocation = mShader->getUniformLocation("uSampler" + std::to_string(samplerIndex));
	}
	mProjectionLocation = mShader->getUniformLocation("uProjection");
	mWorldViewLocation = mShader->getUniformLocation("uWorldView");
}

