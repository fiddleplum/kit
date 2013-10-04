#include "Sprite.h"
#include "../Texture.h"

namespace Gui
{
	Sprite::Sprite()
	{
		std::vector<unsigned int> indices (6);
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;
		model.setIndices(indices);
		updateVertices();
	}

	Sprite::~Sprite()
	{
	}

	Box2i Sprite::getBounds() const
	{
		return Box2i::minSize(model.getPosition(), textureBounds.getSize());
	}

	void Sprite::setPosition(Vector2i position)
	{
		model.setPosition(position);
	}

	void Sprite::setMaxSize(Vector2i maxSize)
	{
		this->maxSize = maxSize;
		updateVertices();
	}

	void Sprite::handleEvent(Event const & event)
	{
	}

	void Sprite::render()
	{
		model.render();
	}

	void Sprite::setTexture(std::string const & filename)
	{
		model.setTexture(filename);
	}

	void Sprite::setTextureBounds(Box2i bounds)
	{
		textureBounds = bounds;
		updateVertices();
	}

	Box2i Sprite::getTextureBounds() const
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
		model.setVertices(vertices);
	}
}

