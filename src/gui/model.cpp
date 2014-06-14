#include "model.h"
#include "../app_p.h"
#include "../resources_p.h"
#include "../texture_p.h"
#include "../shader.h"
#include "../vertex_buffer_object.h"

namespace kit
{
	namespace gui
	{
		Model::Model()
		{
			position = Vector2i(0, 0);

			std::string code [Shader::NumCodeTypes];
			code[Shader::Vertex] +=
				"#version 150\n"
				"uniform ivec2 uWindowSize;\n"
				"uniform ivec2 uPosition;\n"
				"in vec2 aPos;\n"
				"in vec2 aUv;\n"
				"out vec2 vUv;\n"
				"void main()\n"
				"{\n"
				"  gl_Position.x = float(uPosition.x + aPos.x) / float(uWindowSize.x) * 2.0f - 1.0f;\n"
				"  gl_Position.y = 1.0f - float(uPosition.y + aPos.y) / float(uWindowSize.y) * 2.0f;\n"
				"  gl_Position.z = 0;\n"
				"  gl_Position.w = 1;\n"
				"  vUv = aUv;\n"
				"}\n";
			code[Shader::Fragment] +=
				"#version 150\n"
				"uniform ivec2 uTextureSize;\n"
				"uniform sampler2D uSampler;\n"
				"in vec2 vUv;\n"
				"void main()\n"
				"{\n"
				"  gl_FragColor = texture2D(uSampler, vec2(vUv.s / float(uTextureSize.x), vUv.t / float(uTextureSize.y)));\n"
				"}\n";
			shader = app()->getResources()->getShader("guiShader", code);

			vbo.set(new VertexBufferObject);
			vbo->addVertexComponent(shader->getAttributeLocation("aPos"), 0, 2);
			vbo->addVertexComponent(shader->getAttributeLocation("aUv"), sizeof(Vector2f), 2);
			vbo->setBytesPerVertex(sizeof(Vertex));
			vbo->setNumIndicesPerPrimitive(3);

			windowSizeLocation = shader->getUniformLocation("uWindowSize");
			positionLocation = shader->getUniformLocation("uPosition");
			textureSizeLocation = shader->getUniformLocation("uTextureSize");
			samplerLocation = shader->getUniformLocation("uSampler");
		}

		Vector2i Model::getPosition() const
		{
			return position;
		}

		void Model::setPosition(Vector2i position)
		{
			this->position = position;
		}

		void Model::setTexture(std::string const & filename)
		{
			texture = app()->getResources()->getTextureFromFile(filename).as<TextureP>();
		}

		void Model::setVertices(std::vector<Vertex> const & vertices)
		{
			vbo->setVertices((void const *)&vertices[0], sizeof(Vertex) * vertices.size(), false);
		}

		void Model::setIndices(std::vector<unsigned int> const & indices)
		{
			vbo->setIndices(&indices[0], indices.size());
		}

		void Model::render(Vector2i windowSize)
		{
			shader->activate();
			shader->setUniform(windowSizeLocation, windowSize);
			shader->setUniform(positionLocation, position);
			if(texture.isValid())
			{
				texture->activate(0);
				shader->setUniform(textureSizeLocation, texture->getSize());
			}
			shader->setUniform(samplerLocation, 0);
			vbo->render();
		}
	}
}

