#include "sprite.h"
#include "../scene_2d.h"
#include "../texture.h"
#include <vector>

namespace FlatWorld
{
	Sprite::Sprite()
	{
		entity = std::make_shared<Entity2D>();
		std::vector<unsigned int> indices (6);
		indices[0] = 0;
		indices[1] = 3;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 1;
		indices[5] = 0;
		entity->model.setIndices(&indices[0], 6);
		updateVertices();
	}

	Sprite::~Sprite()
	{
	}

	Rectf Sprite::getBounds() const
	{
		return Rectf::minSize(entity->frame.getPosition(), textureBounds.getSize());
	}

	void Sprite::setPosition(Vector2f position)
	{
		entity->frame.setPosition(position);
	}

	void Sprite::setTexture(std::string const & filename)
	{
		entity->model.setTexture(filename);
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

	void Sprite::setFrame(int frame)
	{
		this->frame = frame;
		updateVertices();
	}

	void Sprite::updateVertices()
	{
		Vector2f size;
		Rectf uvBounds;
		size = textureBounds.getSize();
		uvBounds.min = Vector2f(textureBounds.min + Vector2i(textureBounds.getSize()[0] * frame, 0)).scaleInv(entity->model.getTexture()->getSize());
		uvBounds.max = Vector2f(textureBounds.max + Vector2i(textureBounds.getSize()[0] * frame, 0)).scaleInv(entity->model.getTexture()->getSize());
		std::vector<Model2D::Vertex> vertices(4);
		vertices[0].pos.set(0, 0);
		vertices[0].uv.set(uvBounds.min[0], uvBounds.min[1]);
		vertices[1].pos.set(size[0], 0);
		vertices[1].uv.set(uvBounds.max[0], uvBounds.min[1]);
		vertices[2].pos.set(size[0], size[1]);
		vertices[2].uv.set(uvBounds.max[0], uvBounds.max[1]);
		vertices[3].pos.set(0, size[1]);
		vertices[3].uv.set(uvBounds.min[0], uvBounds.max[1]);
		entity->model.setVertices(&vertices[0], sizeof(Model2D::Vertex) * vertices.size());
	}
}

