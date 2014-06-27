#pragma once

#include <kit/texture.h>
#include <kit/ptr.h>
#include <kit/vector.h>
#include <kit/scene/model.h>
#include <string>

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
	}
}

