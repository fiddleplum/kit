#include "scene_model.h"
#include "resources.h"
#include "open_gl.h"
#include "serialize.h"
#include <fstream>
#include <algorithm>

SceneModel::SceneModel()
{
	vertexHasNormal = false;
	vertexHasTangent = false;
	vertexHasColor = false;
	numVertexUVs = 0;
	emitColor = {0, 0, 0};
	diffuseColor = {1, 1, 1, 1};
	specularLevel = 1;
	specularStrength = 0;
	scale = 1;
	vertexBufferObject.setNew();
	vertexBufferObject->setBytesPerVertex(sizeof(Coord3f));
	shaderDirty = true;
	sorted = false;
}

SceneModel::SceneModel(std::string const & filename)
{
	std::fstream in(filename, std::fstream::in | std::fstream::binary);

	// Material
	deserialize(in, emitColor);
	deserialize(in, diffuseColor);
	deserialize(in, specularLevel);
	deserialize(in, specularStrength);

	// Textures
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
		addTextureFromFile(filename, type, uvIndex);
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
	vertices.resize(numVertices * numBytesPerVertex);
	deserialize(in, (void *)&vertices[0], numVertices * numBytesPerVertex);
	setVertices((void const *)&vertices[0], numVertices * numBytesPerVertex);

	// Index format
	unsigned int numIndicesPerPrimitive;
	deserialize(in, numIndicesPerPrimitive);
	setNumIndicesPerPrimitive(numIndicesPerPrimitive);

	// Indices
	std::vector<unsigned int> indices;
	deserialize(in, indices, deserialize);
	setIndices(&indices[0], indices.size());
}

void SceneModel::setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int _numVertexUVs)
{
	numBytesPerVertex = sizeof(Coord3f);
	vertexHasNormal = hasNormal;
	if(vertexHasNormal)
	{
		numBytesPerVertex += sizeof(Coord3f);
	}
	vertexHasTangent = hasTangent;
	if(vertexHasTangent)
	{
		numBytesPerVertex += sizeof(Coord3f);
	}
	vertexHasColor = hasColor;
	if(vertexHasColor)
	{
		numBytesPerVertex += sizeof(Coord4f);
	}
	numVertexUVs = _numVertexUVs;
	numBytesPerVertex += _numVertexUVs * sizeof(Coord2f);
	vertexBufferObject->setBytesPerVertex(numBytesPerVertex);
	shaderDirty = true;
	sorted = false;
}

void SceneModel::setVertices(void const * vertices, unsigned int numBytes)
{
	vertexBufferObject->setVertices(vertices, numBytes, false);
}

void SceneModel::setNumIndicesPerPrimitive(unsigned int num)
{
	vertexBufferObject->setNumIndicesPerPrimitive(num);
}

void SceneModel::setIndices(unsigned int const * indices, unsigned int numIndices)
{
	vertexBufferObject->setIndices(indices, numIndices);
}

void SceneModel::addTexture(Ptr<Texture> texture, std::string const & type, unsigned int uvIndex)
{
	TextureInfo textureInfo;
	textureInfo.texture = texture;
	textureInfo.type = type;
	textureInfo.samplerLocation = -1;
	textureInfo.uvIndex = uvIndex;
	textureInfos.push_back(textureInfo);
	shaderDirty = true;
	sorted = false;
}

void SceneModel::addTextureFromFile(std::string const & filename, std::string const & type, unsigned int uvIndex)
{
	Ptr<Texture> texture = textureCache->load(filename, filename);
	addTexture(texture, type, uvIndex);
}

void SceneModel::clearTextures()
{
	textureInfos.clear();
	shaderDirty = true;
	sorted = false;
}

void SceneModel::setColor(Coord3f const & _emitColor, Coord4f const & _diffuseColor)
{
	emitColor = _emitColor;
	diffuseColor = _diffuseColor;
}

void SceneModel::setSpecular(unsigned int level, float strength)
{
	specularLevel = level;
	specularStrength = strength;
}

float SceneModel::getScale() const
{
	return scale;
}

void SceneModel::setScale(float _scale)
{
	scale = _scale;
}

void SceneModel::render(Matrix44f const & projectionTransform, Matrix44f const & localToCameraTransform, std::vector<Coord3f> const & lightPositions, std::vector<Coord3f> const & lightColors) const
{
	// The render engine handles shader and texture activation.
	if(shaderDirty)
	{
		const_cast<SceneModel *>(this)->updateShader();
	}
	shader->activate();
	shader->setUniform(projectionLocation, projectionTransform);
	shader->setUniform(worldViewLocation, localToCameraTransform);
	shader->setUniform(scaleLocation, scale);
	unsigned int samplerIndex = 0;
	for(unsigned int i = 0; i < textureInfos.size(); i++)
	{
		textureInfos[i].texture->activate(i);
		shader->setUniform(textureInfos[i].samplerLocation, (int)samplerIndex);
		samplerIndex++;
	}
	Texture::deactivateRest(textureInfos.size());
	if(!lightPositions.empty())
	{
		shader->setUniform(lightPositionsLocation, &lightPositions[0], lightPositions.size());
		shader->setUniform(lightColorsLocation, &lightColors[0], lightColors.size());
	}
	shader->setUniform(emitColorLocation, emitColor);
	shader->setUniform(diffuseColorLocation, diffuseColor);
	shader->setUniform(specularLevelLocation, (int)specularLevel);
	shader->setUniform(specularStrengthLocation, specularStrength);
	vertexBufferObject->render();
}

bool SceneModel::needsResorting() const
{
	return !sorted;
}

void SceneModel::resortingDone()
{
	sorted = true;
}

bool SceneModel::operator < (SceneModel const & model) const
{
	if(shader < model.shader)
	{
		return true;
	}
	else if(shader > model.shader)
	{
		return false;
	}
	for(unsigned int i = 0; i < std::min(textureInfos.size(), model.textureInfos.size()); i++)
	{
		if(textureInfos[i].texture < model.textureInfos[i].texture)
		{
			return true;
		}
		else if(textureInfos[i].texture > model.textureInfos[i].texture)
		{
			return false;
		}
	}
	if(textureInfos.size() < model.textureInfos.size())
	{
		return true;
	}
	else if(textureInfos.size() > model.textureInfos.size())
	{
		return false;
	}
	return vertexBufferObject < model.vertexBufferObject;
}

void SceneModel::updateShader()
{
	std::string version = "120";
	std::string attribute = "attribute";
	std::string varyingIn = "varying";
	std::string varyingOut = "varying";
	float glslVersion = glGetGLSLVersion();
	if(glslVersion >= 1.5f)
	{
		version = "150";
		attribute = "in";
		varyingIn = "in";
		varyingOut = "out";
	}
	std::string code[Shader::NumCodeTypes];

	std::vector<std::string> uvIndexStrings;
	for(unsigned int uvIndex = 0; uvIndex < numVertexUVs; uvIndex++)
	{
		uvIndexStrings.push_back(std::to_string(uvIndex));
	}

	/** VERTEX **/
	code[Shader::Vertex] += "#version " + version + "\n";

	// Add the global variables.
	code[Shader::Vertex] += "uniform mat4 uWorldView;\n";
	code[Shader::Vertex] += "uniform mat4 uProjection;\n";
	code[Shader::Vertex] += "uniform float uScale;\n";
	code[Shader::Vertex] += attribute + " vec3 aPosition;\n";
	code[Shader::Vertex] += varyingOut + " vec3 vPosition;\n";
	if(vertexHasNormal)
	{
		code[Shader::Vertex] += attribute + " vec3 aNormal;\n";
		code[Shader::Vertex] += varyingOut + " vec3 vNormal;\n";
	}
	if(vertexHasTangent)
	{
		code[Shader::Vertex] += attribute + " vec3 aTangent;\n";
		code[Shader::Vertex] += varyingOut + " vec3 vTangent;\n";
	}
	if(vertexHasColor)
	{
		code[Shader::Vertex] += attribute + " vec4 aColor;\n";
		code[Shader::Vertex] += varyingOut + " vec4 vColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < numVertexUVs; uvIndex++)
	{
		code[Shader::Vertex] += attribute + " vec2 aUV" + uvIndexStrings[uvIndex] + ";\n";
		code[Shader::Vertex] += varyingOut + " vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
	}

	// Add the main function.
	code[Shader::Vertex] += "void main()\n";
	code[Shader::Vertex] += "{\n";
	code[Shader::Vertex] += "	gl_Position = uProjection * uWorldView * vec4(uScale * aPosition, 1);\n";
	code[Shader::Vertex] += "	vPosition = (uWorldView * vec4(aPosition, 1)).xyz;\n";
	if(vertexHasNormal)
	{
		code[Shader::Vertex] += "	vNormal = (uWorldView * vec4(aNormal, 0)).xyz;\n";
	}
	if(vertexHasTangent)
	{
		code[Shader::Vertex] += "	vTangent = (uWorldView * vec4(aTangent, 0)).xyz;\n";
	}
	if(vertexHasColor)
	{
		code[Shader::Vertex] += "	vColor = aColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < numVertexUVs; uvIndex++)
	{
		code[Shader::Vertex] += "	vUV" + uvIndexStrings[uvIndex] + " = aUV" + uvIndexStrings[uvIndex] + ";\n";
	}
	code[Shader::Vertex] += "}\n";

	/** FRAGMENT **/
	code[Shader::Fragment] += "#version " + version + "\n";

	// Add the global variables.
	code[Shader::Fragment] += varyingIn + " vec3 vPosition;\n";
	code[Shader::Fragment] += "uniform vec3 uEmitColor;\n";
	if(vertexHasNormal)
	{
		code[Shader::Fragment] += "uniform vec3 uLightPositions [" + std::to_string(maxLights) + "];\n";
		code[Shader::Fragment] += "uniform vec3 uLightColors [" + std::to_string(maxLights) + "];\n";
		code[Shader::Fragment] += varyingIn + " vec3 vNormal;\n";
		if(vertexHasTangent)
		{
			code[Shader::Fragment] += varyingIn + " vec3 vTangent;\n";
		}
	}
	if(vertexHasColor)
	{
		code[Shader::Fragment] += varyingIn + " vec4 vColor;\n";
	}
	else
	{
		code[Shader::Fragment] += "uniform vec4 uDiffuseColor;\n";
	}
	for(unsigned int uvIndex = 0; uvIndex < numVertexUVs; uvIndex++)
	{
		code[Shader::Fragment] += varyingIn + " vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
	}
	for(unsigned int samplerIndex = 0; samplerIndex < textureInfos.size(); samplerIndex++)
	{
		code[Shader::Fragment] += "uniform sampler2D uSampler" + std::to_string(samplerIndex) + ";\n";
	}

	// Add the main function.
	code[Shader::Fragment] += "void main()\n";
	code[Shader::Fragment] += "{\n";
	if(vertexHasColor)
	{
		code[Shader::Fragment] += "	vec4 color = vColor;\n";
	}
	else
	{
		code[Shader::Fragment] += "	vec4 color = uDiffuseColor;\n";
	}
	unsigned int samplerIndex = 0;
	for(TextureInfo const & textureInfo : textureInfos)
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
	if(vertexHasNormal)
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
	code[Shader::Fragment] += "	if(gl_FragColor.a == 0)\n";
	code[Shader::Fragment] += "	{\n";
	code[Shader::Fragment] += "		discard;\n";
	code[Shader::Fragment] += "	}\n";
	code[Shader::Fragment] += "	gl_FragColor.rgb += uEmitColor;\n";
	code[Shader::Fragment] += "}\n";

	// Create a unique name for the _shader.
	std::string name;
	name = "Model";
	if(vertexHasNormal)
	{
		name += "n";
	}
	if(vertexHasTangent)
	{
		name += "t";
	}
	if(vertexHasColor)
	{
		name += "c";
	}
	name += std::to_string(numVertexUVs);
	for(TextureInfo const & textureInfo : textureInfos)
	{
		name += textureInfo.type[0] + std::to_string(textureInfo.uvIndex);
	}

	shader = shaderCache->load(name, code);
	sorted = false;

	// Update attribute locations
	vertexBufferObject->clearVertexComponents();
	unsigned int offset = 0;
	vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aPosition"), offset, 3);
	offset += sizeof(Coord3f);
	if(vertexHasNormal)
	{
		vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aNormal"), offset, 3);
		offset += sizeof(Coord3f);
	}
	if(vertexHasTangent)
	{
		vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aTangent"), offset, 3);
		offset += sizeof(Coord3f);
	}
	if(vertexHasColor)
	{
		vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aColor"), offset, 4);
		offset += sizeof(Coord4f);
	}
	for(TextureInfo const & textureInfo : textureInfos)
	{
		vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aUV" + std::to_string(textureInfo.uvIndex)), offset + textureInfo.uvIndex * sizeof(Coord2f), 2);
	}

	// Update uniform locations
	lightPositionsLocation = shader->getUniformLocation("uLightPositions");
	lightColorsLocation = shader->getUniformLocation("uLightColors");
	emitColorLocation = shader->getUniformLocation("uEmitColor");
	diffuseColorLocation = shader->getUniformLocation("uDiffuseColor");
	specularLevelLocation = shader->getUniformLocation("uSpecularLevel");
	specularStrengthLocation = shader->getUniformLocation("uSpecularStrength");
	scaleLocation = shader->getUniformLocation("uScale");
	samplerIndex = 0;
	for(TextureInfo & textureInfo : textureInfos)
	{
		textureInfo.samplerLocation = shader->getUniformLocation("uSampler" + std::to_string(samplerIndex));
	}
	projectionLocation = shader->getUniformLocation("uProjection");
	worldViewLocation = shader->getUniformLocation("uWorldView");

	shaderDirty = false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

/*

Model File Format

strings are UTF-8 encoded. They have the number of bytes prepended as an unsigned int. They are not null-terminated.
bools is 8 bits.
ints, unsigned ints, and floats are 32 bits.
lists begin with an unsigned int indicating their length and then the elements tightly packed.

model:
material - the material
bool - has normal
bool - has tangent
bool - has color
unsigned int - num uvs
vertex list - list of vertex components (vertices all contatenated, tightly packed)
unsigned int - number of indices per primitive
int list - list of indices that make up triangles

material:
float[3] - emmisive color (0..1)
float[4] - diffuse color (0..1)
unsigned int - shininess (>= 1)
float - shininess strength (>= 0)
texture list - list of textures

texture:
string - filename
string - type
int - uv index

vertex format:
float[3] - position
float[3] - normal (if it has one)
float[3] - tangent (if it has one)
float[4] - color (if it has one)
float[2] list - uvs *** Note: this list is not prepended with a length.

*/

