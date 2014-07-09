#pragma once

#include "texture.h"
#include "shader.h"
#include "scene/model.h"

namespace kit
{
	namespace resources
	{
		Ptr<Texture> getBlankTexture (std::string const & name, Vector2i size);

		Ptr<Texture> getTextureFromFile (std::string const & filename);

		Ptr<Shader> getShader (std::string const & name, std::string code []);

		Ptr<scene::Model> getBlankModel (std::string const & name);

		Ptr<scene::Model> getModelFromFile (std::string const & filename);

		void initialize ();

		void finalize ();
	}
}

