#include "sprite_p.h"
#include "model.h"
#include "../resources_p.h"
#include "../window_p.h"
#include <kit/texture.h>

namespace kit
{
	namespace gui
	{
		OwnPtr<Sprite> Sprite::create ()
		{
			return OwnPtr<SpriteP> (new SpriteP);
		}

		SpriteP::SpriteP ()
		{
			_model.set(new Model);
			std::vector<unsigned int> indices (6);
			indices[0] = 0;
			indices[1] = 3;
			indices[2] = 2;
			indices[3] = 2;
			indices[4] = 1;
			indices[5] = 0;
			_model->setIndices(indices);
			updateVertices();
		}

		Recti SpriteP::getBounds () const
		{
			return Recti::minSize(_model->getPosition(), _textureBounds.getSize());
		}

		void SpriteP::setPosition (Vector2i position)
		{
			_model->setPosition(position);
		}

		void SpriteP::setMaxSize (Vector2i maxSize)
		{
			_maxSize = maxSize;
			updateVertices();
		}

		UsePtr<Texture> SpriteP::getTexture () const
		{
			return _model->getTexture();
		}

		void SpriteP::setTexture (UsePtr<Texture> texture)
		{
			_model->setTexture(texture);
		}

		void SpriteP::setTexture (std::string const & filename)
		{
			_model->setTexture(resources::getTextureFromFile(filename));
		}

		Recti SpriteP::getTextureBounds () const
		{
			return _textureBounds;
		}

		void SpriteP::setTextureBounds (Recti bounds)
		{
			_textureBounds = bounds;
			updateVertices();
		}

		void SpriteP::handleEvent (Event const & event)
		{
			if(getBounds().containsEx(event.window.as<WindowP>()->getCursorPosition()))
			{
				event.window.as<WindowP>()->consumeCursor();
			}
		}

		void SpriteP::render (Vector2i windowSize)
		{
			_model->render(windowSize);
		}

		void SpriteP::updateVertices()
		{
			Vector2i size;
			size[0] = std::min(_textureBounds.getSize()[0], _maxSize[0]);
			size[1] = std::min(_textureBounds.getSize()[1], _maxSize[1]);
			std::vector<Model::Vertex> vertices (4);
			vertices[0].pos.set(0, 0);
			vertices[0].uv.set((float)_textureBounds.min[0], (float)_textureBounds.min[1]);
			vertices[1].pos.set((float)size[0], 0);
			vertices[1].uv.set((float)_textureBounds.min[0] + (float)size[0], (float)_textureBounds.min[1]);
			vertices[2].pos.set((float)size[0], (float)size[1]);
			vertices[2].uv.set((float)_textureBounds.min[0] + (float)size[0], (float)_textureBounds.min[1] + (float)size[1]);
			vertices[3].pos.set(0, (float)size[1]);
			vertices[3].uv.set((float)_textureBounds.min[0], (float)_textureBounds.min[1] + (float)size[1]);
			_model->setVertices(vertices);
		}
	}
}

