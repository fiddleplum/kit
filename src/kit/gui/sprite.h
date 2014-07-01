#pragma once

#include "../ptr.h"
#include "../texture.h"
#include "../event.h"
#include "widget.h"
#include <string>

namespace kit
{
	namespace gui
	{
		// TODO: Add ability to have a texture mask for determining the shape of the sprite when it comes to cursor clicks, etc.

		class Model;

		class Sprite : public Widget
		{
		public:
			Sprite ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			Ptr<Texture> getTexture () const;

			void setTexture (Ptr<Texture> texture);

			void setTexture (std::string const & filename);

			Recti getTextureBounds () const;

			void setTextureBounds (Recti bounds);

			void handleEvent (Event const & event);

			void render (Vector2i windowSize);

		private:
			void updateVertices ();

			OwnPtr<Model> _model;
			Recti _textureBounds;
			Vector2i _maxSize;
		};
	}
}

