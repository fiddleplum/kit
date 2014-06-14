#pragma once

#include "widget_p.h"
#include <kit/gui/sprite.h>

namespace kit
{
	namespace gui
	{
		class Model;

		class SpriteP : virtual public Sprite, virtual public WidgetP
		{
		public:
			SpriteP ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			Ptr<Texture> getTexture () const override;

			void setNewTexture (Vector2i size) override;

			void setTexture (std::string const & filename) override;

			Recti getTextureBounds () const override;

			void setTextureBounds (Recti bounds) override;

			bool handleEvent (Event const & event, bool cursorIsValid) override;

			void render (Vector2i windowSize) override;

		private:
			void updateVertices ();

			OwnPtr<Model> model;
			Recti textureBounds;
			Vector2i maxSize;
		};
	}
}

