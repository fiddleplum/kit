#include "gui_model.h"
#include "texture.h"
#include "shader.h"
#include "vertex_buffer_object.h"
#include "open_gl.h"
#include "shader_cache.h"

GuiModel::GuiModel()
{
	updateShader();

	vbo.setNew();
	vbo->addVertexComponent(shader->getAttributeLocation("aPos"), 0, 2);
	vbo->addVertexComponent(shader->getAttributeLocation("aUv"), sizeof(Vector2f), 2);
	vbo->setBytesPerVertex(sizeof(Vertex));
	vbo->setNumIndicesPerPrimitive(3);

	windowSizeLocation = shader->getUniformLocation("uWindowSize");
	positionLocation = shader->getUniformLocation("uPosition");
	textureSizeLocation = shader->getUniformLocation("uTextureSize");
	samplerLocation = shader->getUniformLocation("uSampler");
}

Vector2i GuiModel::getPosition() const
{
	return position;
}

void GuiModel::setPosition(Vector2i position)
{
	this->position = position;
}

Ptr<Texture> GuiModel::getTexture() const
{
	return texture;
}

void GuiModel::setTexture(Ptr<Texture> texture)
{
	this->texture = texture;
}

void GuiModel::setVertices(std::vector<Vertex> const & vertices)
{
	vbo->setVertices((void const *)&vertices[0], sizeof(Vertex) * vertices.size(), false);
}

void GuiModel::setIndices(std::vector<unsigned int> const & indices)
{
	vbo->setIndices(&indices[0], indices.size());
}

void GuiModel::render(Vector2i windowSize)
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

void GuiModel::updateShader()
{
	if(!ShaderCache::instance()->has("guiShader"))
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
		code[Shader::Vertex] +=
			"#version " + version + "\n"
			"uniform ivec2 uWindowSize;\n"
			"uniform ivec2 uPosition;\n"
			+ attribute + " ivec2 aPos;\n"
			+ attribute + " ivec2 aUv;\n"
			+ varyingOut + " ivec2 vUv;\n"
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
			+ varyingIn + " ivec2 vUv;\n"
			"void main()\n"
			"{\n"
			"  gl_FragColor = texture2D(uSampler, vec2(float(vUv.s) / float(uTextureSize.x), float(vUv.t) / float(uTextureSize.y)));\n"
			"}\n";
		shader = ShaderCache::instance()->load("guiShader", code);
	}
	else
	{
		shader = ShaderCache::instance()->get("guiShader");
	}
}

