#pragma once

#include <kit/ptr.h>
#include <kit/vector.h>
#include <string>

namespace kit
{
	class Texture;
	class Shader;

	namespace scene
	{
		class Model;
	}

	class Resources
	{
	public:
		virtual Ptr<Texture> getNewTexture (std::string const & name, Vector2i size) = 0;

		virtual Ptr<Texture> getTextureFromFile (std::string const & filename) = 0;

		virtual Ptr<scene::Model> getNewModel (std::string const & name) = 0;

		virtual Ptr<scene::Model> getModelFromFile (std::string const & filename) = 0;
	};
}

