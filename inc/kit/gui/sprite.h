#pragma once

#include <kit/ptr.h>
#include <kit/gui/widget.h>
#include <kit/texture.h>
#include <string>

namespace kit
{
	namespace gui
	{
		class Model;

		class Sprite : public Widget
		{
		public:
			Sprite ();

			Recti getBounds () const;

			void setPosition (Vector2i position);

			void setMaxSize (Vector2i maxSize);

			Ptr<Texture> getTexture () const;

			void setTexture (Ptr<Texture> texture);

			void setTexture (std::string const & filename);

			void setTextureBounds (Recti bounds);

			Recti getTextureBounds () const;

			void handleEvent (Event const & event);

			void render (Vector2i windowSize);

		private:
			void updateVertices ();

			OwnPtr<Model> _model;
			Recti _textureBounds;
			Vector2i _maxSize;

			friend class WidgetContainer;
		};
	}
}

