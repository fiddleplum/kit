#include "Model.h"
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include "Serialize.h"
#include "SerializeStdString.h"
#include "SerializeStdVector.h"
#include "StringUtil.h"
#include <fstream>
#include <stdexcept>

#include <iostream>

Model::Model(Scene * scene)
{
	mScene = scene;
	mVertexHasNormal = false;
	mVertexHasTangent = false;
	mVertexHasColor = false;
	mNumVertexUVs = 0;
	mEmitColor = Vector4f::zero();
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
	Vector4f emitColor;
	deserialize(in, emitColor[0]);
	deserialize(in, emitColor[1]);
	deserialize(in, emitColor[2]);
	deserialize(in, emitColor[3]);
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

void Model::addTexture(std::string const & filename, std::string const & type, unsigned int uvIndex)
{
	TextureInfo textureInfo;
	textureInfo.texture = mScene->getTextureManager().get(filename, filename);
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

void Model::setColor(Vector4f const & emitColor, Vector4f const & diffuseColor)
{
	mEmitColor = emitColor;
	mDiffuseColor = diffuseColor;
}

void Model::setSpecular(unsigned int level, float strength)
{
	mSpecularLevel = level;
	mSpecularStrength = strength;
}

void Model::render(Camera const * camera) const
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
	std::string code [Shader::NumTypes];

	std::vector<std::string> uvIndexStrings;
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		uvIndexStrings.push_back(std::to_string(uvIndex));
	}

	/** VERTEX **/
	code[Shader::Vertex] += "#version 120\n";

	// Add the global variables.
	code[Shader::Vertex] += "uniform mat4 uWorldView;\n";
	code[Shader::Vertex] += "uniform mat4 uProjection;\n";
	code[Shader::Vertex] += "attribute vec3 aPosition;\n";
	code[Shader::Vertex] += "varying vec3 vPosition;\n";
	if(mVertexHasNormal)
	{
		code[Shader::Vertex] += "attribute vec3 aNormal;\n";
		code[Shader::Vertex] += "varying vec3 vNormal;\n";
	}
	if(mVertexHasTangent)
	{
		code[Shader::Vertex] += "attribute vec3 aTangent;\n";
		code[Shader::Vertex] += "varying vec3 vTangent;\n";
	}
	if(mVertexHasColor)
	{
		code[Shader::Vertex] += "attribute vec4 aColor;\n";
		code[Shader::Vertex] += "varying vec4 vColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		code[Shader::Vertex] += "attribute vec2 aUV" + uvIndexStrings[uvIndex] + ";\n";
		code[Shader::Vertex] += "varying vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
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
	code[Shader::Fragment] += "#version 120\n";

	// Add the global variables.
	code[Shader::Fragment] += "varying vec3 vPosition;\n";
	if(mVertexHasNormal)
	{
		code[Shader::Fragment] += "varying vec3 vNormal;\n";
	}
	if(mVertexHasTangent)
	{
		code[Shader::Fragment] += "varying vec3 vTangent;\n";
	}
	if(mVertexHasColor)
	{
		code[Shader::Fragment] += "varying vec4 vColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < mNumVertexUVs; uvIndex++)
	{
		code[Shader::Fragment] += "varying vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
	}
	code[Shader::Fragment] += "uniform vec4 uEmitColor;\n";
	code[Shader::Fragment] += "uniform vec4 uDiffuseColor;\n";
	unsigned int samplerIndex = 0;
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		code[Shader::Fragment] += "uniform sampler2D uSampler" + std::to_string(samplerIndex) + ";\n";
		samplerIndex++;
	}

	// Add the main function.
	code[Shader::Fragment] += "void main()\n";
	code[Shader::Fragment] += "{\n";
	code[Shader::Fragment] += "	vec4 color = uEmitColor;\n";
	if(mVertexHasColor)
	{
		code[Shader::Fragment] += "	color = vColor;\n";
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
	code[Shader::Fragment] += "	gl_FragColor = color;\n";
	code[Shader::Fragment] += "}\n";

	std::cout << code[Shader::Vertex] << std::endl;
	std::cout << code[Shader::Fragment] << std::endl;

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

	mShader = mScene->getShaderManager().get(name, code);
	mNeedsResorting = true;

	// Update attribute locations
	mVertexBufferObject->clearVertexComponents();
	unsigned int offset = 0;
	mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aPosition"), offset, VertexBufferObject::Float, 3);
	offset += sizeof(Vector3f);
	if(mVertexHasNormal)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aNormal"), offset, VertexBufferObject::Float, 3);
		offset += sizeof(Vector3f);
	}
	if(mVertexHasTangent)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aTangent"), offset, VertexBufferObject::Float, 3);
		offset += sizeof(Vector3f);
	}
	if(mVertexHasColor)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aColor"), offset, VertexBufferObject::Float, 4);
		offset += sizeof(Vector4f);
	}
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		mVertexBufferObject->addVertexComponent(mShader->getAttributeLocation("aUV" + std::to_string(textureInfo.uvIndex)), offset + textureInfo.uvIndex * sizeof(Vector2f), VertexBufferObject::Float, 2);
	}

	// Update uniform locations
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

