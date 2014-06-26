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
		UsePtr<Texture> getBlankTexture (std::string const & name, Vector2i size);

		UsePtr<Texture> getTextureFromFile (std::string const & filename);

		UsePtr<scene::Model> getBlankModel (std::string const & name);

		UsePtr<scene::Model> getModelAsSprite (std::string const & name, std::string const & textureFilename, Recti textureCoords);

		UsePtr<scene::Model> getModelFromFile (std::string const & filename);
	}
}

