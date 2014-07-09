#include "model.h"
#include "camera.h"
#include "../resources.h"
#include "../vertex_buffer_object.h"
#include "../serialize.h"
#include "../serialize_std_string.h"
#include "../serialize_std_vector.h"
#include <fstream>

namespace kit
{
	namespace scene
	{
		Model::Model ()
		{
			_vertexHasNormal = false;
			_vertexHasTangent = false;
			_vertexHasColor = false;
			_numVertexUVs = 0;
			_emitColor = Vector3f(0, 0, 0);
			_diffuseColor = Vector4f(1, 1, 1, 1);
			_specularLevel = 1;
			_specularStrength = 0;
			_scale = 1;
			_vertexBufferObject.set(new VertexBufferObject);
			_vertexBufferObject->setBytesPerVertex(sizeof(Vector3f));
			updateShader();
			_sorted = false;
		}

		Model::Model (std::string const & filename)
		{
			std::fstream in (filename, std::fstream::in | std::fstream::binary);

			// Material
			deserialize(in, _emitColor[0]);
			deserialize(in, _emitColor[1]);
			deserialize(in, _emitColor[2]);
			deserialize(in, _diffuseColor[0]);
			deserialize(in, _diffuseColor[1]);
			deserialize(in, _diffuseColor[2]);
			deserialize(in, _diffuseColor[3]);
			deserialize(in, _specularLevel);
			deserialize(in, _specularStrength);

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
			vertices.resize(numVertices * _numBytesPerVertex);
			deserialize(in, (void *)&vertices[0], numVertices * _numBytesPerVertex);
			setVertices((void const *)&vertices[0], numVertices * _numBytesPerVertex);

			// Index format
			unsigned int numIndicesPerPrimitive;
			deserialize(in, numIndicesPerPrimitive);
			setNumIndicesPerPrimitive(numIndicesPerPrimitive);

			// Indices
			std::vector<unsigned int> indices;
			deserialize<unsigned int>(in, indices, deserialize);
			setIndices(&indices[0], indices.size());
		}

		void Model::setVertexFormat (bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs)
		{
			_numBytesPerVertex = sizeof(Vector3f); // position
			_vertexHasNormal = hasNormal;
			if(_vertexHasNormal)
			{
				_numBytesPerVertex += sizeof(Vector3f);
			}
			_vertexHasTangent = hasTangent;
			if(_vertexHasTangent)
			{
				_numBytesPerVertex += sizeof(Vector3f);
			}
			_vertexHasColor = hasColor;
			if(_vertexHasColor)
			{
				_numBytesPerVertex += sizeof(Vector4f);
			}
			_numVertexUVs = numVertexUVs;
			_numBytesPerVertex += _numVertexUVs * sizeof(Vector2f);
			_vertexBufferObject->setBytesPerVertex(_numBytesPerVertex);
			updateShader();
		}

		void Model::setVertices (void const * vertices, unsigned int numBytes)
		{
			_vertexBufferObject->setVertices(vertices, numBytes, false);
		}

		void Model::setNumIndicesPerPrimitive (unsigned int num)
		{
			_vertexBufferObject->setNumIndicesPerPrimitive(num);
		}

		void Model::setIndices (unsigned int const * indices, unsigned int numIndices)
		{
			_vertexBufferObject->setIndices(indices, numIndices);
		}

		Ptr<kit::Texture> Model::getTexture (unsigned int textureIndex) const
		{
			if(textureIndex < _textureInfos.size())
			{
				return _textureInfos[textureIndex].texture;
			}
			else
			{
				return Ptr<Texture>();
			}
		}

		void Model::addTexture (Ptr<Texture> texture, std::string const & type, unsigned int uvIndex)
		{
			TextureInfo textureInfo;
			textureInfo.texture = texture;
			textureInfo.type = type;
			textureInfo.samplerLocation = -1;
			textureInfo.uvIndex = uvIndex;
			_textureInfos.push_back(textureInfo);
			updateShader();
		}

		void Model::addTexture (std::string const & filename, std::string const & type, unsigned int uvIndex)
		{
			Ptr<Texture> texture = resources::getTextureFromFile(filename);
			addTexture(texture, type, uvIndex);
		}

		void Model::clearTextures ()
		{
			_textureInfos.clear();
			updateShader();
		}

		void Model::setColor (Vector3f const & emitColor, Vector4f const & diffuseColor)
		{
			_emitColor = emitColor;
			_diffuseColor = diffuseColor;
		}

		void Model::setSpecular (unsigned int level, float strength)
		{
			_specularLevel = level;
			_specularStrength = strength;
		}

		float Model::getScale () const
		{
			return _scale;
		}

		void Model::setScale (float scale)
		{
			_scale = scale;
		}		

		void Model::render (Ptr<Camera> camera, Ptr<Entity> entity, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const
		{
			// The render engine handles shader and texture activation.
			_shader->activate();
			_shader->setUniform(_projectionLocation, camera->getCameraToNdcTransform());
			_shader->setUniform(_worldViewLocation, camera->getWorldToCameraTransform() * entity->getLocalToWorldTransform());
			_shader->setUniform(_scaleLocation, _scale);
			unsigned int samplerIndex = 0;
			for(unsigned int i = 0; i < _textureInfos.size(); i++)
			{
				_textureInfos[i].texture->activate(i);
				_shader->setUniform(_textureInfos[i].samplerLocation, (int)samplerIndex);
				samplerIndex++;
			}
			Texture::deactivateRest(_textureInfos.size());
			if(!lightPositions.empty())
			{
				_shader->setUniform(_lightPositionsLocation, &lightPositions[0], lightPositions.size());
				_shader->setUniform(_lightColorsLocation, &lightColors[0], lightColors.size());
			}
			_shader->setUniform(_emitColorLocation, _emitColor);
			_shader->setUniform(_diffuseColorLocation, _diffuseColor);
			_shader->setUniform(_specularLevelLocation, (int)_specularLevel);
			_shader->setUniform(_specularStrengthLocation, _specularStrength);
			_vertexBufferObject->render();
		}

		bool Model::needsResorting () const
		{
			return !_sorted;
		}

		void Model::resortingDone ()
		{
			_sorted = true;
		}

		bool Model::operator < (Model const & model) const
		{
			if(_shader < model._shader)
			{
				return true;
			}
			else if(_shader > model._shader)
			{
				return false;
			}
			for(unsigned int i = 0; i < std::min(_textureInfos.size(), model._textureInfos.size()); i++)
			{
				if(_textureInfos[i].texture < model._textureInfos[i].texture)
				{
					return true;
				}
				else if(_textureInfos[i].texture > model._textureInfos[i].texture)
				{
					return false;
				}
			}
			if(_textureInfos.size() < model._textureInfos.size())
			{
				return true;
			}
			else if(_textureInfos.size() > model._textureInfos.size())
			{
				return false;
			}
			return _vertexBufferObject < model._vertexBufferObject;
		}

		void Model::updateShader ()
		{
			std::string code [Shader::NumCodeTypes];

			std::vector<std::string> uvIndexStrings;
			for(unsigned int uvIndex = 0; uvIndex < _numVertexUVs; uvIndex++)
			{
				uvIndexStrings.push_back(std::to_string(uvIndex));
			}

			/** VERTEX **/
			code[Shader::Vertex] += "#version 150\n";

			// Add the global variables.
			code[Shader::Vertex] += "uniform mat4 uWorldView;\n";
			code[Shader::Vertex] += "uniform mat4 uProjection;\n";
			code[Shader::Vertex] += "uniform float uScale;\n";
			code[Shader::Vertex] += "in vec3 aPosition;\n";
			code[Shader::Vertex] += "out vec3 vPosition;\n";
			if(_vertexHasNormal)
			{
				code[Shader::Vertex] += "in vec3 aNormal;\n";
				code[Shader::Vertex] += "out vec3 vNormal;\n";
			}
			if(_vertexHasTangent)
			{
				code[Shader::Vertex] += "in vec3 aTangent;\n";
				code[Shader::Vertex] += "out vec3 vTangent;\n";
			}
			if(_vertexHasColor)
			{
				code[Shader::Vertex] += "in vec4 aColor;\n";
				code[Shader::Vertex] += "out vec4 vColor;\n";
			}
			for(unsigned int uvIndex = 0; uvIndex < _numVertexUVs; uvIndex++)
			{
				code[Shader::Vertex] += "in vec2 aUV" + uvIndexStrings[uvIndex] + ";\n";
				code[Shader::Vertex] += "out vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
			}

			// Add the main function.
			code[Shader::Vertex] += "void main()\n";
			code[Shader::Vertex] += "{\n";
			code[Shader::Vertex] += "	gl_Position = uProjection * uWorldView * vec4(uScale * aPosition, 1);\n";
			code[Shader::Vertex] += "	vPosition = (uWorldView * vec4(aPosition, 1)).xyz;\n";
			if(_vertexHasNormal)
			{
				code[Shader::Vertex] += "	vNormal = (uWorldView * vec4(aNormal, 0)).xyz;\n";
			}
			if(_vertexHasTangent)
			{
				code[Shader::Vertex] += "	vTangent = (uWorldView * vec4(aTangent, 0)).xyz;\n";
			}
			if(_vertexHasColor)
			{
				code[Shader::Vertex] += "	vColor = aColor;\n";
			}
			for(unsigned int uvIndex = 0; uvIndex < _numVertexUVs; uvIndex++)
			{
				code[Shader::Vertex] += "	vUV" + uvIndexStrings[uvIndex] + " = aUV" + uvIndexStrings[uvIndex] + ";\n";
			}
			code[Shader::Vertex] += "}\n";

			/** FRAGMENT **/
			code[Shader::Fragment] += "#version 150\n";

			// Add the global variables.
			code[Shader::Fragment] += "in vec3 vPosition;\n";
			code[Shader::Fragment] += "uniform vec3 uEmitColor;\n";
			if(_vertexHasNormal)
			{
				code[Shader::Fragment] += "uniform vec3 uLightPositions [" + std::to_string(maxLights) + "];\n";
				code[Shader::Fragment] += "uniform vec3 uLightColors [" + std::to_string(maxLights) + "];\n";
				code[Shader::Fragment] += "in vec3 vNormal;\n";
				if(_vertexHasTangent)
				{
					code[Shader::Fragment] += "in vec3 vTangent;\n";
				}
			}
			if(_vertexHasColor)
			{
				code[Shader::Fragment] += "in vec4 vColor;\n";
			}
			else
			{
				code[Shader::Fragment] += "uniform vec4 uDiffuseColor;\n";
			}
			for(unsigned int uvIndex = 0; uvIndex < _numVertexUVs; uvIndex++)
			{
				code[Shader::Fragment] += "in vec2 vUV" + uvIndexStrings[uvIndex] + ";\n";
			}
			for(unsigned int samplerIndex = 0; samplerIndex < _textureInfos.size(); samplerIndex++)
			{
				code[Shader::Fragment] += "uniform sampler2D uSampler" + std::to_string(samplerIndex) + ";\n";
			}

			// Add the main function.
			code[Shader::Fragment] += "void main()\n";
			code[Shader::Fragment] += "{\n";
			if(_vertexHasColor)
			{
				code[Shader::Fragment] += "	vec4 color = vColor;\n";
			}
			else
			{
				code[Shader::Fragment] += "	vec4 color = uDiffuseColor;\n";
			}
			unsigned int samplerIndex = 0;
			for(TextureInfo const & textureInfo : _textureInfos)
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
			if(_vertexHasNormal)
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

			// Create a unique name for the _shader.
			std::string name;
			name = "Model";
			if(_vertexHasNormal)
			{
				name += "n";
			}
			if(_vertexHasTangent)
			{
				name += "t";
			}
			if(_vertexHasColor)
			{
				name += "c";
			}
			name += std::to_string(_numVertexUVs);
			for(TextureInfo const & textureInfo : _textureInfos)
			{
				name += textureInfo.type[0] + std::to_string(textureInfo.uvIndex);
			}

			_shader = resources::getShader(name, code);
			_sorted = false;

			// Update attribute locations
			_vertexBufferObject->clearVertexComponents();
			unsigned int offset = 0;
			_vertexBufferObject->addVertexComponent(_shader->getAttributeLocation("aPosition"), offset, 3);
			offset += sizeof(Vector3f);
			if(_vertexHasNormal)
			{
				_vertexBufferObject->addVertexComponent(_shader->getAttributeLocation("aNormal"), offset, 3);
				offset += sizeof(Vector3f);
			}
			if(_vertexHasTangent)
			{
				_vertexBufferObject->addVertexComponent(_shader->getAttributeLocation("aTangent"), offset, 3);
				offset += sizeof(Vector3f);
			}
			if(_vertexHasColor)
			{
				_vertexBufferObject->addVertexComponent(_shader->getAttributeLocation("aColor"), offset, 4);
				offset += sizeof(Vector4f);
			}
			for(TextureInfo const & textureInfo : _textureInfos)
			{
				_vertexBufferObject->addVertexComponent(_shader->getAttributeLocation("aUV" + std::to_string(textureInfo.uvIndex)), offset + textureInfo.uvIndex * sizeof(Vector2f), 2);
			}

			// Update uniform locations
			_lightPositionsLocation = _shader->getUniformLocation("uLightPositions");
			_lightColorsLocation = _shader->getUniformLocation("uLightColors");
			_emitColorLocation = _shader->getUniformLocation("uEmitColor");
			_diffuseColorLocation = _shader->getUniformLocation("uDiffuseColor");
			_specularLevelLocation = _shader->getUniformLocation("uSpecularLevel");
			_specularStrengthLocation = _shader->getUniformLocation("uSpecularStrength");
			_scaleLocation = _shader->getUniformLocation("uScale");
			samplerIndex = 0;
			for(TextureInfo & textureInfo : _textureInfos)
			{
				textureInfo.samplerLocation = _shader->getUniformLocation("uSampler" + std::to_string(samplerIndex));
			}
			_projectionLocation = _shader->getUniformLocation("uProjection");
			_worldViewLocation = _shader->getUniformLocation("uWorldView");
		}
	}
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

