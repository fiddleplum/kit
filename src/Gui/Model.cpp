#include "Model.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../VertexBufferObject.h"
#include "../App.h"

namespace Gui
{
	Model::Model()
	{
		App::getShaderManager().get(
	}

	void Model::setTexture(std::string const & filename)
	{
		App::getTextureManager().get(filename, filename);
	}

	void Model::setVertices(std::vector<Vertex> const & vertices)
	{
	}

	void Model::setNumFrames(int numFrames)
	{
		this->numFrames = numFrames;
	}

	void Model::setFrame(int frame)
	{
	}

	void Model::render(Vector2i windowSize)
	{
	}

	void Model::initShader()
	{
		std::string code [Shader::NumCodeTypes];
		code[Shader::Vertex] +=
			"#version 120\n"
			"uniform vec2 uWindowSize;\n"
			"attribute vec2 aPos;\n"
			"attribute vec2 aUv;\n"
			"varying vec2 vUv;\n"
			"void main()\n"
			"{\n"
			"  gl_Position.x = aPos.x / uWindowSize.x * 2.0f - 1.0f;\n"
			"  gl_Position.y = 1.0f - aPos.y / uWindowSize.y * 2.0f;\n"
			"  gl_Position.z = gl_Position.w = 0;\n"
			"  vUv = aUv;\n"
			"}\n";
		code[Shader::Fragment] +=
			"#version 120\n"
			"uniform vec2 uTextureSize;\n"
			"uniform sampler2D uSampler;\n"
			"varying vec2 vUv;\n"
			"void main()\n"
			"{\n"
			"  gl_FragColor = texture2D(uSampler, vUv / uTextureSize);\n"
			"}\n";
	}
}

