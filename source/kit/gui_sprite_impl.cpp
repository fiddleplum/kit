#include "gui_sprite_impl.h"
#include "gui_model.h"
#include "texture.h"
#include "texture_cache.h"
#include <algorithm>

GuiSpriteImpl::GuiSpriteImpl()
{
	model.setNew();
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

Recti GuiSpriteImpl::getBounds() const
{
	return Recti(position, position + textureBounds.max - textureBounds.min);
}

void GuiSpriteImpl::setPosition(Vector2i position)
{
	this->position = position;
	model->setPosition(position);
	updateVertices();
}

Recti GuiSpriteImpl::getTextureBounds() const
{
	return textureBounds;
}

void GuiSpriteImpl::setTextureBounds(Recti bounds)
{
	textureBounds = bounds;
	updateVertices();
}

void GuiSpriteImpl::setTexture(std::string const & name, Image const & image)
{
	model->setTexture(TextureCache::instance()->load(name, image));
}

void GuiSpriteImpl::setTexture(std::string const & filename)
{
	model->setTexture(TextureCache::instance()->load(filename, filename));
}

Image GuiSpriteImpl::getImage() const
{
	return model->getTexture()->getImage();
}

void GuiSpriteImpl::render(Vector2i windowSize) const
{
	model->render(windowSize);
}

void GuiSpriteImpl::updateVertices()
{
	Recti bounds{position, position + textureBounds.max - textureBounds.min};
	std::vector<GuiModel::Vertex> vertices(4);
	vertices[0].pos = {bounds.min[0], bounds.min[1]};
	vertices[0].uv = {textureBounds.min[0], textureBounds.min[1]};
	vertices[1].pos = {bounds.max[0], bounds.min[1]};
	vertices[1].uv = {textureBounds.max[0], textureBounds.min[1]};
	vertices[2].pos = {bounds.max[0], bounds.max[1]};
	vertices[2].uv = {textureBounds.max[0], textureBounds.max[1]};
	vertices[3].pos = {bounds.min[0], bounds.max[1]};
	vertices[3].uv = {textureBounds.min[0], textureBounds.max[1]};
	model->setVertices(vertices);
}

