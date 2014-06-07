#pragma once

#include "widget_internal.h"
#include <kit/gui/sprite.h>
#include "model.h"

namespace kit
{
	namespace gui
	{
		class SpriteInternal : virtual public Sprite, virtual public WidgetInternal
		{
		public:
			SpriteInternal ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			void setTexture (std::string const & filename) override;

			void setTextureBounds (Recti bounds) override;

			Recti getTextureBounds () const override;

			bool handleEvent (Event const & event, bool cursorIsValid) override;

			void render (Vector2i windowSize) override;

			void updateVertices ();

		private:
			OwnPtr<Model> model;
			Recti textureBounds;
			Vector2i maxSize;
		};
	}
}

