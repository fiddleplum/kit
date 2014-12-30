#include "gui_sprite.h"
#include "gui_model.h"
#include "texture_cache.h"
#include <algorithm>

GuiSprite::GuiSprite()
{
	model.create();
	std::vector<unsigned int> indices(6);
	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 0;
	model->setIndices(indices);
	updateVertices();
}

Recti GuiSprite::getBounds() const
{
	return Recti::minSize(position, textureBounds.getSize());
}

void GuiSprite::setPosition(Vector2i position)
{
	this->position = position;
	model->setPosition(position);
	bounds = Recti::minSize(position, textureBounds.getSize());
	updateVertices();
}

Recti GuiSprite::getTextureBounds() const
{
	return textureBounds;
}

void GuiSprite::setTextureBounds(Recti bounds)
{
	textureBounds = bounds;
	bounds = Recti::minSize(position, textureBounds.getSize());
	updateVertices();
}

Ptr<Texture> GuiSprite::getTexture() const
{
	return model->getTexture();
}

void GuiSprite::setTexture(Ptr<Texture> texture)
{
	model->setTexture(texture);
}

void GuiSprite::setTexture(std::string const & filename)
{
	setTexture(TextureCache::instance()->load(filename, filename));
}

void GuiSprite::render(Vector2i windowSize) const
{
	model->render(windowSize);
}

void GuiSprite::updateVertices()
{
	std::vector<GuiModel::Vertex> vertices(4);
	vertices[0].pos.set(bounds.min[0], bounds.min[1]);
	vertices[0].uv.set(textureBounds.min[0], textureBounds.min[1]);
	vertices[1].pos.set(bounds.max[0], bounds.min[1]);
	vertices[1].uv.set(textureBounds.max[0], textureBounds.min[1]);
	vertices[2].pos.set(bounds.max[0], bounds.max[1]);
	vertices[2].uv.set(textureBounds.max[0], textureBounds.max[1]);
	vertices[3].pos.set(bounds.min[0], bounds.max[1]);
	vertices[3].uv.set(textureBounds.min[0], textureBounds.max[1]);
	model->setVertices(vertices);
}

