#include "gui_sprite.h"
#include "gui_model.h"
#include "resource_cache.h"
#include "cursor.h"
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
	updateVertices();
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

void GuiSprite::setClipBounds(Recti clipBounds)
{
	this->clipBounds = clipBounds;
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
	setTexture(ResourceCache::getTextureFromFile(filename));
}

bool GuiSprite::handleEvent(Event const & event, Ptr<Cursor> cursor)
{
	if(event.type == Event::MouseButton)
	{
		auto mbEvent = event.as<MouseButtonEvent>();
		if(mbEvent.pressed == true && visibleBounds.containsEx(cursor->getPosition()))
		{
			return true;
		}
	}
	else if(event.type == Event::MouseWheel)
	{
		auto mwEvent = event.as<MouseWheelEvent>();
		if(visibleBounds.containsEx(cursor->getPosition()))
		{
			return true;
		}
	}
	return false;
}

void GuiSprite::render(Vector2i windowSize) const
{
	model->render(windowSize);
}

void GuiSprite::updateVertices()
{
	visibleBounds = Recti::minSize(position, textureBounds.getSize()).intersectedWith(clipBounds);
	Recti vertexBounds = Recti::minSize(visibleBounds.min - position, visibleBounds.getSize());
	Recti uvBounds = Recti::minSize(textureBounds.min + vertexBounds.min, visibleBounds.getSize());
	std::vector<GuiModel::Vertex> vertices(4);
	vertices[0].pos.set(vertexBounds.min[0], vertexBounds.min[1]);
	vertices[0].uv.set(uvBounds.min[0], uvBounds.min[1]);
	vertices[1].pos.set(vertexBounds.max[0], vertexBounds.min[1]);
	vertices[1].uv.set(uvBounds.max[0], uvBounds.min[1]);
	vertices[2].pos.set(vertexBounds.max[0], vertexBounds.max[1]);
	vertices[2].uv.set(uvBounds.max[0], uvBounds.max[1]);
	vertices[3].pos.set(vertexBounds.min[0], vertexBounds.max[1]);
	vertices[3].uv.set(uvBounds.min[0], uvBounds.max[1]);
	model->setVertices(vertices);
}

