#include "WidgetSprite.h"
#include "../Texture.h"

WidgetSprite::WidgetSprite()
{
}

WidgetSprite::~WidgetSprite()
{
}

Box2i WidgetSprite::getBounds() const
{
	return bounds;
}

void WidgetSprite::setPosition(Vector2i position)
{
}

void WidgetSprite::setMaxSize(Vector2i maxSize)
{
}

void WidgetSprite::handleEvent(Event const & event)
{
}

void WidgetSprite::render()
{
	texture->activate(0);
}

