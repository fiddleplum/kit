#pragma once

#include "../interval.h"
#include <memory>
#include <string>

namespace kit
{
	class Model2D;

	namespace Flat
	{
		class Sprite
		{
		public:
			Sprite();
			~Sprite();

			Vector2f getSize() const;
			void setTexture(std::string const & filename);
			void setTextureBounds(Recti bounds);
			Recti getTextureBounds() const;
			void setFrame(int frame);

			std::shared_ptr<Model2D> getModel() const { return model; }

		private:
			void updateVertices();

			std::shared_ptr<Model2D> model;
			Recti textureBounds;
			int frame;
		};
	}
}

