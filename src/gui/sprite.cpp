#include "model.h"
#include "../texture.h"
#include <kit/gui/sprite.h>
#include <vector>

namespace kit
{
	namespace gui
	{
		Sprite::Sprite()
		{
			model.set(new Model);
			std::vector<unsigned int> indices (6);
			indices[0] = 0;
			indices[1] = 3;
			indices[2] = 2;
			indices[3] = 2;
			indices[4] = 1;
			indices[5] = 0;
			model->setIndices(indices);
			updateVertices();
		}

		Sprite::~Sprite()
		{
			model.setNull();
		}

		Recti Sprite::getBounds() const
		{
			return Recti::minSize(model->getPosition(), textureBounds.getSize());
		}

		void Sprite::setPosition(Vector2i position)
		{
			model->setPosition(position);
		}

		void Sprite::setMaxSize(Vector2i maxSize)
		{
			this->maxSize = maxSize;
			updateVertices();
		}

		bool Sprite::handleEvent(Event const &, bool)
		{
			return true; // change this later
		}

		void Sprite::render(Vector2i windowSize)
		{
			model->render(windowSize);
		}

		void Sprite::setTexture(std::string const & filename)
		{
			model->setTexture(filename);
		}

		void Sprite::setTextureBounds(Recti bounds)
		{
			textureBounds = bounds;
			updateVertices();
		}

		Recti Sprite::getTextureBounds() const
		{
			return textureBounds;
		}

		void Sprite::updateVertices()
		{
			Vector2i size;
			size[0] = std::min(textureBounds.getSize()[0], maxSize[0]);
			size[1] = std::min(textureBounds.getSize()[1], maxSize[1]);
			std::vector<Model::Vertex> vertices (4);
			vertices[0].pos.set(0, 0);
			vertices[0].uv.set((float)textureBounds.min[0], (float)textureBounds.min[1]);
			vertices[1].pos.set((float)size[0], 0);
			vertices[1].uv.set((float)textureBounds.min[0] + (float)size[0], (float)textureBounds.min[1]);
			vertices[2].pos.set((float)size[0], (float)size[1]);
			vertices[2].uv.set((float)textureBounds.min[0] + (float)size[0], (float)textureBounds.min[1] + (float)size[1]);
			vertices[3].pos.set(0, (float)size[1]);
			vertices[3].uv.set((float)textureBounds.min[0], (float)textureBounds.min[1] + (float)size[1]);
			model->setVertices(vertices);
		}
	}
}

