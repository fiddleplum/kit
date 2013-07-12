#include "Model.h"
#include "Serialize.h"
#include "SerializeStdString.h"
#include "SerializeStdVector.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include "Shader.h"
#include "StringUtil.h"
#include <istream>
#include <stdexcept>

Model::Model(Config const & config)
{
	construct(config);
}

Model::Model()
{
	mVertexHasNormal = false;
	mVertexHasTangent = false;
	mVertexHasColor = false;
	mNumVertexUVs = 0;
	mColor = Vector4f(1, 1, 1, 1);
	mShaderNeedsUpdate = true;
	mVertexBufferObject = new VertexBufferObject();
}

Model::Model(std::istream & in)
{
	Config config;

	// Material
	config.hasBaseColor = true;
	deserialize(in, config.baseColor[0]);
	deserialize(in, config.baseColor[1]);
	deserialize(in, config.baseColor[2]);
	deserialize(in, config.baseColor[3]);
	deserialize(in, config.specularLevel);
	deserialize(in, config.specularStrength);
	deserialize<Config::Texture>(in, config.textures, [] (std::istream & in, Config::Texture & textureConfig)
	{
		deserialize(in, textureConfig.filename);
		std::string type;
		deserialize(in, type);
		if(type == "diffuse")
		{
			textureConfig.type = Config::Texture::Diffuse;
		}
		else if(type == "normal")
		{
			textureConfig.type = Config::Texture::Normal;
		}
		else if(type == "reflection")
		{
			textureConfig.type = Config::Texture::Reflection;
		}
		deserialize(in, textureConfig.uvIndex);
	});

	// Vertex format
	deserialize(in, config.vertexHasNormal);
	deserialize(in, config.vertexHasTangent);
	deserialize(in, config.vertexHasColor);
	deserialize(in, config.numVertexUVs);
	deserialize(in, config.numVertices);
	int numBytesPerVertex = 3 * sizeof(float);
	if(config.vertexHasNormal)
	{
		numBytesPerVertex += 3 * sizeof(float);
	}
	if(config.vertexHasTangent)
	{
		numBytesPerVertex += 3 * sizeof(float);
	}
	if(config.vertexHasColor)
	{
		numBytesPerVertex += 4 * sizeof(float);
	}
	numBytesPerVertex += 2 * sizeof(float) * config.numVertexUVs;
	std::vector<unsigned char> vertices;
	vertices.resize(config.numVertices * numBytesPerVertex);
	deserialize(in, (void *)&vertices[0], config.numVertices * numBytesPerVertex);
	config.vertices = (void const*)&vertices[0];

	// Index format
	std::vector<unsigned int> indices;
	deserialize(in, config.numIndicesPerPrimitive);
	deserialize<unsigned int>(in, indices, deserialize);
	config.indices = (unsigned int const *)&indices[0];
	config.numIndices = indices.size();

	construct(config);
}

Model::~Model()
{
	delete mVertexBufferObject;
}

std::shared_ptr<Shader const> const & Model::getShader() const
{
	if(mShaderNeedsUpdate)
	{
		const_cast<Model *>(this)->updateShader();
		const_cast<Model *>(this)->mShaderNeedsUpdate = false;
	}
	return mShader;
}

void Model::setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs)
{
	mVertexHasNormal = hasNormal;
	mVertexHasTangent = hasTangent;
	mVertexHasColor = hasColor;
	mNumVertexUVs = numVertexUVs;
	unsigned int bytesPerVertex = sizeof(Vector3f);
	if(mVertexHasNormal)
	{
		bytesPerVertex += sizeof(Vector3f);
	}
	if(mVertexHasTangent)
	{
		bytesPerVertex += sizeof(Vector3f);
	}
	if(mVertexHasColor)
	{
		bytesPerVertex += sizeof(Vector4f);
	}
	bytesPerVertex += numVertexUVs * 2 * sizeof(Vector2f);
	mVertexBufferObject->setBytesPerVertex(bytesPerVertex);
	mShaderNeedsUpdate = true;
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

void Model::render() const
{
	/// NOTE: Make shaders, textures, etc, manage their own historicity, so that when you call texture.activate, it automatically checks if the same texture is already activated.
	mShader->activate();
	unsigned int samplerIndex = 0;
	for(TextureInfo const & textureInfo : mTextureInfos)
	{
		textureInfo.texture->activate(samplerIndex);
		mShader->setUniform(textureInfo.samplerLocation, (int)samplerIndex);
		samplerIndex++;
	}
	mVertexBufferObject->render();
}

void Model::construct(Config const & config)
{
#ifdef DEBUG
	for(Config::Texture const & textureConfig : config.textures)
	{
		if(textureConfig.type == Config::Texture::Normal && (!config.vertexHasNormal || !config.vertexHasTangent))
		{
			throw std::runtime_error("Normal texture specified without both normal and tangent vertex components.");
		}
		if(textureConfig.uvIndex == -1 && (textureConfig.type == Config::Texture::Normal || textureConfig.type == Config::Texture::Normal))
		{
			throw std::runtime_error("Texture type requires a uv.");
		}
		if(textureConfig.uvIndex >= config.numVertexUVs)
		{
			throw std::runtime_error("Texture has higher uvIndex than highest vertex uv.");
		}
	}
#endif

	setupShader(config);

	VertexBufferObject::Config vboConfig;

	// Configure the textures.
	unsigned int samplerIndex = 0;
	for(Config::Texture const & textureConfig : config.textures)
	{
		TextureInfo textureInfo;
		textureInfo.texture = gTextureManager->get(textureConfig.filename);
		textureInfo.samplerLocation = mShader->getUniformLocation("uSampler" + std::to_string(samplerIndex));
		textureInfo.uvIndex = textureConfig.uvIndex;
		samplerIndex++;
	}

	// Configure the attributes.
	unsigned int offset = 0;
	vboConfig.vertexComponents.emplace_back(mShader->getAttributeLocation("aPosition"), offset, VertexBufferObject::Config::VertexComponent::Float, 3);
	if(config.vertexHasNormal)
	{
		vboConfig.vertexComponents.emplace_back(mShader->getAttributeLocation("aNormal"), offset, VertexBufferObject::Config::VertexComponent::Float, 3);
		offset += sizeof(Vector3f);
	}
	if(config.vertexHasTangent)
	{
		vboConfig.vertexComponents.emplace_back(mShader->getAttributeLocation("aTangent"), offset, VertexBufferObject::Config::VertexComponent::Float, 3);
		offset += sizeof(Vector3f);
	}
	if(config.vertexHasColor)
	{
		vboConfig.vertexComponents.emplace_back(mShader->getAttributeLocation("aColor"), offset, VertexBufferObject::Config::VertexComponent::Float, 4);
		offset += sizeof(Vector4f);
	}
	for(Config::Texture const & textureConfig : config.textures)
	{
		vboConfig.vertexComponents.emplace_back(mShader->getAttributeLocation("aUV" + std::to_string(textureConfig.uvIndex)), offset + textureConfig.uvIndex * sizeof(Vector2f), VertexBufferObject::Config::VertexComponent::Float, 2);
	}
	offset += config.numVertexUVs * sizeof(Vector2f);

	vboConfig.vertices = config.vertices;
	vboConfig.numVertices = config.numVertices;
	vboConfig.bytesPerVertex = offset;
	vboConfig.dynamic = false;

	vboConfig.indices = config.indices;
	vboConfig.numIndices = config.numIndices;
	vboConfig.numIndicesPerPrimitive = config.numIndicesPerPrimitive;
	mVertexBufferObject = new VertexBufferObject(vboConfig);
}

void Model::setupShader(Config const & config)
{
	/** VERTEX **/
	std::string vertexCode;
	vertexCode += "#version 120\n";

	// Add the global variables.
	vertexCode += "uniform mat4 uWorldView;\n";
	vertexCode += "uniform mat4 uProjection;\n";
	vertexCode += "attribute vec3 aPosition;\n";
	vertexCode += "varying vec3 vPosition;\n";
	if(config.vertexHasNormal)
	{
		vertexCode += "attribute vec3 aNormal;\n";
		vertexCode += "varying vec3 vNormal;\n";
	}
	if(config.vertexHasColor)
	{
		vertexCode += "attribute vec4 aColor;\n";
		vertexCode += "varying vec4 vColor;\n";
	}
	if(config.vertexHasTangent)
	{
		vertexCode += "attribute vec3 aTangent;\n";
		vertexCode += "varying vec3 vTangent;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < config.numVertexUVs; uvIndex++)
	{
		vertexCode += "attribute vec2 aUV" + std::to_string(uvIndex) + ";\n";
		vertexCode += "varying vec2 vUV" + std::to_string(uvIndex) + ";\n";
	}

	// Add the main function.
	vertexCode += "void main()\n";
	vertexCode += "{\n";
	vertexCode += "	gl_Position = uProjection * uWorldView * aPosition;\n";
	vertexCode += "	vPosition = uWorldView * aPosition;\n";
	if(config.vertexHasNormal)
	{
		vertexCode += "	vNormal = uWorldView * aNormal;\n";
	}
	if(config.vertexHasTangent)
	{
		vertexCode += "	vTangent = uWorldView * aTangent;\n";
	}
	if(config.vertexHasColor)
	{
		vertexCode += "	vColor = aColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < config.numVertexUVs; uvIndex++)
	{
		vertexCode += "	vUV" + std::to_string(uvIndex) + " = aUV" + std::to_string(uvIndex) + ";\n";
	}
	vertexCode += "}\n";

	/** FRAGMENT **/
	std::string fragmentCode;
	fragmentCode += "#version 120\n";

	// Add the global variables.
	vertexCode += "varying vec3 vPosition;\n";
	if(config.vertexHasNormal)
	{
		vertexCode += "varying vec3 vNormal;\n";
	}
	if(config.vertexHasColor)
	{
		vertexCode += "varying vec4 vColor;\n";
	}
	if(config.vertexHasTangent)
	{
		vertexCode += "varying vec3 vTangent;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < config.numVertexUVs; uvIndex++)
	{
		vertexCode += "varying vec2 vUV" + std::to_string(uvIndex) + ";\n";
	}
	unsigned int samplerIndex = 0;
	for(Config::Texture const & textureConfig : config.textures)
	{
		vertexCode += "uniform sampler2D uSampler" + std::to_string(samplerIndex) + ";\n";
		samplerIndex++;
	}

	// Add the main function.
	vertexCode += "void main()\n";
	vertexCode += "{\n";
	vertexCode += "	color = vec4(0,0,0,0);\n";
	if(config.vertexHasColor)
	{
		vertexCode += "	color = vColor;\n";
	}
	samplerIndex = 0;
	for(Config::Texture const & textureConfig : config.textures)
	{
		if(textureConfig.type == Config::Texture::Diffuse)
		{
			vertexCode += "	vec4 textureColor" + std::to_string(samplerIndex) + " = texture2D(uSampler" + std::to_string(samplerIndex) + ", vUV" + std::to_string(textureConfig.uvIndex) + ");\n";
			vertexCode += "	color = (1.0f - textureColor" + std::to_string(samplerIndex) + ".w) * color + textureColor" + std::to_string(samplerIndex) + ".w * textureColor" + std::to_string(samplerIndex) + ";\n";
		}
		else if(textureConfig.type == Config::Texture::Normal)
		{
		}
		else if(textureConfig.type == Config::Texture::Reflection)
		{
		}
		samplerIndex++;
	}
	vertexCode += "	gl_Color = color;\n";
	vertexCode += "}\n";

	mShader = std::make_shared<Shader>(vertexCode, fragmentCode);
}

