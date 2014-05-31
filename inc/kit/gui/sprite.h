#pragma once

#include <kit/gui/widget.h>
#include <kit/ptr.h>

namespace kit
{
	namespace gui
	{
		class Model;

		class Sprite : public Widget
		{
		public:
			Sprite();
			virtual ~Sprite();

			Recti getBounds() const override;
			void setPosition(Vector2i position) override;
			void setMaxSize(Vector2i maxSize) override;
			bool handleEvent(Event const & event, bool cursorIsValid) override;
			void render(Vector2i windowSize) override;

			void setTexture(std::string const & filename);
			void setTextureBounds(Recti bounds);
			Recti getTextureBounds() const;

		private:
			void updateVertices();

			OwnPtr<Model> model;
			Recti textureBounds;
			Vector2i maxSize;
		};
	}
}

