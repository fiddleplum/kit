#pragma once

#include "texture.h"
#include "scene/model.h"
#include "shader.h"

namespace kit
{
	namespace resources
	{
		Ptr<Texture> getBlankTexture (std::string const & name, Vector2i size);

		Ptr<Texture> getTextureFromFile (std::string const & filename);

		Ptr<scene::Model> getBlankModel (std::string const & name);

		Ptr<scene::Model> getModelAsSprite (std::string const & name, std::string const & textureFilename, Recti textureCoords);

		Ptr<scene::Model> getModelFromFile (std::string const & filename);

		Ptr<Shader> getShader (std::string const & name, std::string code []);

		void initialize ();

		void finalize ();
	}
}

