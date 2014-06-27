#include <kit/gui/sprite.h>
#include <kit/window.h>
#include <kit/cursor.h>
#include <kit/texture.h>
#include "model.h"
#include "../resources_p.h"

namespace kit
{
	namespace gui
	{
		Sprite::Sprite ()
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

		Recti Sprite::getBounds () const
		{
			return Recti::minSize(_model->getPosition(), _textureBounds.getSize());
		}

		void Sprite::setPosition (Vector2i position)
		{
			_model->setPosition(position);
		}

		void Sprite::setMaxSize (Vector2i maxSize)
		{
			_maxSize = maxSize;
			updateVertices();
		}

		Ptr<Texture> Sprite::getTexture () const
		{
			return _model->getTexture();
		}

		void Sprite::setTexture (Ptr<Texture> texture)
		{
			_model->setTexture(texture);
		}

		void Sprite::setTexture (std::string const & filename)
		{
			_model->setTexture(resources::getTextureFromFile(filename));
		}

		Recti Sprite::getTextureBounds () const
		{
			return _textureBounds;
		}

		void Sprite::setTextureBounds (Recti bounds)
		{
			_textureBounds = bounds;
			updateVertices();
		}

		void Sprite::handleEvent (Event const & event)
		{
			if(getBounds().containsEx(event.window->cursor()->position()))
			{
				event.window->cursor()->consume();
			}
		}

		void Sprite::render (Vector2i windowSize)
		{
			_model->render(windowSize);
		}

		void Sprite::updateVertices()
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

