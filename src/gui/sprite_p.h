#pragma once

#include "widget_p.h"
#include <kit/gui/sprite.h>

namespace kit
{
	namespace gui
	{
		class Model;

		// TODO: Add ability to have a texture mask for determining the shape of the sprite when it comes to cursor clicks, etc.

		class SpriteP : virtual public Sprite, virtual public WidgetP
		{
		public:
			SpriteP ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			Ptr<Texture> getTexture () const override;

			void setTexture (Ptr<Texture> texture) override;

			void setTexture (std::string const & filename) override;

			Recti getTextureBounds () const override;

			void setTextureBounds (Recti bounds) override;

			void handleEvent (Event const & event) override;

			void render (Vector2i windowSize) override;

		private:
			void updateVertices ();

			OwnPtr<Model> _model;
			Recti _textureBounds;
			Vector2i _maxSize;
		};
	}
}

