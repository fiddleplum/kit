#include "model.h"
#include "../resources.h"
#include "../vertex_buffer_object.h"
#include "../open_gl.h"

namespace kit
{
	namespace gui
	{
		Model::Model()
		{
			updateShader();

			_vbo.create();
			_vbo->addVertexComponent(_shader->getAttributeLocation("aPos"), 0, 2);
			_vbo->addVertexComponent(_shader->getAttributeLocation("aUv"), sizeof(Vector2f), 2);
			_vbo->setBytesPerVertex(sizeof(Vertex));
			_vbo->setNumIndicesPerPrimitive(3);

			_windowSizeLocation = _shader->getUniformLocation("uWindowSize");
			_positionLocation = _shader->getUniformLocation("uPosition");
			_textureSizeLocation = _shader->getUniformLocation("uTextureSize");
			_samplerLocation = _shader->getUniformLocation("uSampler");
		}

		Vector2i Model::getPosition() const
		{
			return _position;
		}

		void Model::setPosition(Vector2i position)
		{
			_position = position;
		}

		Ptr<Texture> Model::getTexture () const
		{
			return _texture;
		}

		void Model::setTexture(Ptr<Texture> texture)
		{
			_texture = texture;
		}

		void Model::setVertices(std::vector<Vertex> const & vertices)
		{
			_vbo->setVertices((void const *)&vertices[0], sizeof(Vertex) * vertices.size(), false);
		}

		void Model::setIndices(std::vector<unsigned int> const & indices)
		{
			_vbo->setIndices(&indices[0], indices.size());
		}

		void Model::render(Vector2i windowSize)
		{
			_shader->activate();
			_shader->setUniform(_windowSizeLocation, windowSize);
			_shader->setUniform(_positionLocation, _position);
			if(_texture.isValid())
			{
				_texture->activate(0);
				_shader->setUniform(_textureSizeLocation, _texture->getSize());
			}
			_shader->setUniform(_samplerLocation, 0);
			_vbo->render();
		}

		void Model::updateShader()
		{
			std::string version = "120";
			std::string attribute = "attribute";
			std::string varyingIn = "varying";
			std::string varyingOut = "varying";
			float glslVersion = glGetGLSLVersion();
			if (glslVersion >= 1.5f)
			{
				version = "150";
				attribute = "in";
				varyingIn = "in";
				varyingOut = "out";
			}
			std::string code[Shader::NumCodeTypes];
			code[Shader::Vertex] +=
				"#version " + version + "\n"
				"uniform ivec2 uWindowSize;\n"
				"uniform ivec2 uPosition;\n"
				+ attribute + " vec2 aPos;\n"
				+ attribute + " vec2 aUv;\n"
				+ varyingOut + " vec2 vUv;\n"
				"void main()\n"
				"{\n"
				"  gl_Position.x = float(uPosition.x + aPos.x) / float(uWindowSize.x) * 2.0f - 1.0f;\n"
				"  gl_Position.y = 1.0f - float(uPosition.y + aPos.y) / float(uWindowSize.y) * 2.0f;\n"
				"  gl_Position.z = 0;\n"
				"  gl_Position.w = 1;\n"
				"  vUv = aUv;\n"
				"}\n";
			code[Shader::Fragment] +=
				"#version " + version + "\n"
				"uniform ivec2 uTextureSize;\n"
				"uniform sampler2D uSampler;\n"
				+ varyingIn + " vec2 vUv;\n"
				"void main()\n"
				"{\n"
				"  gl_FragColor = texture2D(uSampler, vec2(vUv.s / float(uTextureSize.x), vUv.t / float(uTextureSize.y)));\n"
				"}\n";
			_shader = resources::getShader("guiShader", code);
		}
	}
}

