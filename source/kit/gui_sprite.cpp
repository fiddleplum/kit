#include "gui_sprite.h"
#include "gui_model.h"
#include "texture_cache.h"
#include <algorithm>

GuiSprite::GuiSprite()
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

Recti GuiSprite::getBounds() const
{
	return Recti(position, position + textureBounds.max - textureBounds.min);
}

void GuiSprite::setPosition(Coord2i position)
{
	this->position = position;
	model->setPosition(position);
	updateVertices();
}

void GuiSprite::setSize(Coord2i size)
{
	// does nothing
}

Recti GuiSprite::getTextureBounds() const
{
	return textureBounds;
}

void GuiSprite::setTextureBounds(Recti bounds)
{
	textureBounds = bounds;
	updateVertices();
}

void GuiSprite::setTexture(Ptr<Texture> texture)
{
	model->setTexture(texture);
}

void GuiSprite::setTexture(std::string const & filename)
{
	model->setTexture(TextureCache::instance()->load(filename, filename));
}

Ptr<Texture> GuiSprite::getTexture() const
{
	return model->getTexture();
}

bool GuiSprite::handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)
{
	if(event.is(Event::MouseButton))
	{
		auto mbEvent = event.as<MouseButtonEvent>();
		if(mbEvent.pressed && cursorIsOver(cursorPosition, cursorPositionIsValid))
		{
			return true;
		}
	}
	else if(event.is(Event::MouseWheel))
	{
		if(cursorIsOver(cursorPosition, cursorPositionIsValid))
		{
			return true;
		}
	}
	return false;
}

void GuiSprite::render(Coord2i windowSize) const
{
	model->render(windowSize);
}

void GuiSprite::updateVertices()
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

