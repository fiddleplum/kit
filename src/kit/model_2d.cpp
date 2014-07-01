#include "model_2d.h"
#include "camera_2d.h"
#include "app_internal.h"
#include "shader.h"
#include "texture_internal.h"
#include "vertex_buffer_object.h"
#include "serialize.h"
#include "serialize_std_string.h"
#include "serialize_std_vector.h"
#include <kit/string_util.h>
#include <fstream>
#include <stdexcept>

namespace kit
{
	Model2D::Model2D()
	{
		z = 0;
		scale.set(1.0f, 1.0f);
		color.set(1.0f, 1.0f, 1.0f);
		numBytesPerVertex = sizeof(Vector2f) + sizeof(Vector2f);
		vertexBufferObject = new VertexBufferObject();
		vertexBufferObject->setBytesPerVertex(numBytesPerVertex);
		updateShader();
	}

	Model2D::~Model2D()
	{
		delete vertexBufferObject;
	}

	void Model2D::setZ(int newZ)
	{
		z = newZ;
		sorted = false;
	}

	void Model2D::setScale(Vector2f newScale)
	{
		scale = newScale;
	}

	void Model2D::setTexture(std::string const & filename)
	{
		texture = app()->getTextureManager()->get(filename, filename);
		sorted = false;
	}

	void Model2D::setColor(Vector3f newColor)
	{
		color = newColor;
	}

	void Model2D::setVertices(void const * vertices, unsigned int numBytes)
	{
		vertexBufferObject->setVertices(vertices, numBytes, false);
	}

	void Model2D::setNumIndicesPerPrimitive(unsigned int num)
	{
		vertexBufferObject->setNumIndicesPerPrimitive(num);
	}

	void Model2D::setIndices(unsigned int const * indices, unsigned int numIndices)
	{
		vertexBufferObject->setIndices(indices, numIndices);
	}

	void Model2D::load(std::string const & filename)
	{
		std::fstream in (filename, std::fstream::in | std::fstream::binary);

		// Texture
		std::string textureFilename;
		deserialize(in, textureFilename);
		setTexture(textureFilename);
		deserialize(in, color[0]);
		deserialize(in, color[1]);
		deserialize(in, color[2]);

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
		deserialize<unsigned int>(in, indices, deserialize);
		setIndices(&indices[0], indices.size());
	}

	void Model2D::render(std::shared_ptr<Camera2D const> camera, Matrix33f const & transform) const
	{
		// The render engine handles shader and texture activation.
		shader->activate();
		shader->setUniform(viewSizeLocation, camera->getViewSize());
		shader->setUniform(scaleLocation, getScale());
		shader->setUniform(worldViewLocation, camera->getViewTransform() * transform);
		texture->activate(0);
		shader->setUniform(samplerLocation, 0);
		shader->setUniform(colorLocation, color);
		vertexBufferObject->render();
	}

	bool Model2D::isSorted() const
	{
		return sorted;
	}

	void Model2D::setSorted()
	{
		sorted = true;
	}

	bool Model2D::operator <(Model2D const & model) const
	{
		if(z < model.z)
		{
			return true;
		}
		else if (z > model.z)
		{
			return false;
		}
		else if(shader < model.shader)
		{
			return true;
		}
		else if(shader > model.shader)
		{
			return false;
		}
		if(texture < model.texture)
		{
			return true;
		}
		else if(texture > model.texture)
		{
			return false;
		}
		return vertexBufferObject < model.vertexBufferObject;
	}

	void Model2D::updateShader()
	{
		std::string code [Shader::NumCodeTypes];

		/** VERTEX **/
		code[Shader::Vertex] += "#version 150\n";

		// Add the global variables.
		code[Shader::Vertex] += "uniform mat3 uWorldView;\n";
		code[Shader::Vertex] += "uniform vec2 uViewSize;\n";
		code[Shader::Vertex] += "uniform vec2 uScale;\n";
		code[Shader::Vertex] += "in vec2 aPosition;\n";
		code[Shader::Vertex] += "out vec2 vPosition;\n";
		code[Shader::Vertex] += "in vec2 aUV;\n";
		code[Shader::Vertex] += "out vec2 vUV;\n";

		// Add the main function.
		code[Shader::Vertex] += "void main()\n";
		code[Shader::Vertex] += "{\n";
		code[Shader::Vertex] += "	vPosition = (uWorldView * uScale * vec3(aPosition, 1)).xy;\n";
		code[Shader::Vertex] += "	gl_Position = vec4(vPosition / uViewSize, 0, 1);\n";
		code[Shader::Vertex] += "	vUV = aUV;\n";
		code[Shader::Vertex] += "}\n";

		/** FRAGMENT **/
		code[Shader::Fragment] += "#version 150\n";

		// Add the global variables.
		code[Shader::Fragment] += "in vec3 vPosition;\n";
		code[Shader::Fragment] += "in vec2 vUV;\n";
		code[Shader::Fragment] += "uniform sampler2D uSampler;\n";
		code[Shader::Fragment] += "uniform vec3 uColor;\n";

		// Add the main function.
		code[Shader::Fragment] += "void main()\n";
		code[Shader::Fragment] += "{\n";
		code[Shader::Fragment] += "	vec4 textureColor = texture2D(uSampler, vUV);\n";
		code[Shader::Fragment] += "	gl_FragColor = vec4(textureColor.rgb * uColor, textureColor.a);\n";
		code[Shader::Fragment] += "}\n";

		// Create a unique name for the shader.
		std::string name;
		name = "Model2D";

		shader = app()->getShaderManager()->get(name, code);
		sorted = false;

		// Update attribute locations
		vertexBufferObject->clearVertexComponents();
		unsigned int offset = 0;
		vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aPosition"), offset, 3);
		offset += sizeof(Vector3f);
		vertexBufferObject->addVertexComponent(shader->getAttributeLocation("aUV"), offset, 2);

		// Update uniform locations
		scaleLocation = shader->getUniformLocation("uScale");
		viewSizeLocation = shader->getUniformLocation("uViewSize");
		worldViewLocation = shader->getUniformLocation("uWorldView");
		samplerLocation = shader->getUniformLocation("uSampler");
		colorLocation = shader->getUniformLocation("uColor");
	}
}

