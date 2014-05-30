#pragma once

#include <kit/widget.h>
#include "model.h"
#include <memory>

namespace kit
{
	namespace app
	{
		class Sprite : public Widget
		{
		public:
			Sprite();
			virtual ~Sprite();

			Recti getBounds() const override;
			void setPosition(Vector2i position) override;
			void setMaxSize(Vector2i maxSize) override;
			void handleEvent(Event const & event) override;
			void render() override;

			void setTexture(std::string const & filename);
			void setTextureBounds(Recti bounds);
			Recti getTextureBounds() const;

		private:
			void updateVertices();

			Model model;
			Recti textureBounds;
			Vector2i maxSize;
		};
	}
}

