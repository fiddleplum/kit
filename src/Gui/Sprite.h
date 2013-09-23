#pragma once

#include "Widget.h"
#include <memory>

class Texture;

class WidgetSprite : public Widget
{
public:
	WidgetSprite();
	virtual ~WidgetSprite();

	Box2i getBounds() const override;
	void setPosition(Vector2i position) override;
	void setMaxSize(Vector2i maxSize) override;
	void handleEvent(Event const & event) override;
	void render() override;

private:
	std::shared_ptr<Texture> texture;
	Box2i bounds;
};

