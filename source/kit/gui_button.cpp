#include "gui_button.h"
#include "gui_sprite.h"
#include "time.h"
#include "cursor.h"

GuiButton::GuiButton()
{
	sprite.create();
	type = Hold;
	hovered = false;
	pressed = false;
	shotInterval = .5f;
}

Recti GuiButton::getBounds() const
{
	return sprite->getBounds();
}

void GuiButton::setPosition(Vector2i position)
{
	sprite->setPosition(position);
}

void GuiButton::setClipBounds(Recti clipBounds)
{
	sprite->setClipBounds(clipBounds);
}

bool GuiButton::handleEvent(Event const & event, Ptr<Cursor> cursor)
{
	if(!hovered)
	{
		if((event.type == Event::MouseMove && getBounds().containsEx(cursor->getPosition())) || hasFocus())
		{
			hovered = true;
			setSpriteTextureBoundsFromState();
		}
	}
	else // hovered
	{
		if((event.type == Event::MouseMove && !getBounds().containsEx(cursor->getPosition())) && !hasFocus())
		{
			hovered = false;
			setSpriteTextureBoundsFromState();
		}
		if(!pressed)
		{
			bool willPress = false;
			if(event.type == Event::MouseButton)
			{
				MouseButtonEvent const & mbe = event.as<MouseButtonEvent>();
				if(mbe.button == mbe.Left && mbe.pressed)
				{
					willPress = true;
				}
			}
			else if(hasFocus() && event.type == Event::Keyboard)
			{
				KeyboardEvent const & ke = event.as<KeyboardEvent>();
				if((ke.key == ke.Enter || ke.key == ke.Space) && ke.pressed)
				{
					willPress = true;
				}
			}
			if(willPress)
			{
				pressed = true;
				timePressed = Time::getSecondsSinceStart();
				setSpriteTextureBoundsFromState();
				if(pressFunction != nullptr)
				{
					pressFunction();
				}
			}
		}
		else // pressed
		{
			bool willUnpress = false;
			if(event.type == Event::Update && Time::getSecondsSinceStart() - timePressed > shotInterval)
			{
				willUnpress = true;
			}
			else if(event.type == Event::MouseButton)
			{
				MouseButtonEvent const & mbe = event.as<MouseButtonEvent>();
				if(mbe.button == mbe.Left)
				{
					if(type == Toggle && mbe.pressed)
					{
						willUnpress = true;
					}
					else if(type == Hold && !mbe.pressed)
					{
						willUnpress = true;
					}
				}
			}
			else if(hasFocus() && event.type == Event::Keyboard)
			{
				KeyboardEvent const & ke = event.as<KeyboardEvent>();
				if((ke.key == ke.Enter || ke.key == ke.Space) && ke.pressed)
				{
					if(type == Toggle && ke.pressed)
					{
						willUnpress = true;
					}
					else if(type == Hold && !ke.pressed)
					{
						willUnpress = true;
					}
				}
			}
			if(willUnpress)
			{
				pressed = false;
				setSpriteTextureBoundsFromState();
				if(unpressFunction != nullptr)
				{
					unpressFunction();
				}
			}
		}
	}
	return sprite->handleEvent(event, cursor);
}

void GuiButton::render(Vector2i windowSize) const
{
	sprite->render(windowSize);
}

Ptr<Texture> GuiButton::getTexture() const
{
	return sprite->getTexture();
}

void GuiButton::setTexture(Ptr<Texture> texture)
{
	sprite->setTexture(texture);
}

void GuiButton::setTexture(std::string const & filename)
{
	sprite->setTexture(filename);
}

void GuiButton::setTextureBoundsOfDefaultState(Recti bounds)
{
	defaultTextureBounds = bounds;
	sprite->setTextureBounds(bounds);
}

void GuiButton::setType(Type type)
{
	this->type = type;
}

void GuiButton::setShotInterval(float interval)
{
	shotInterval = interval;
}

void GuiButton::setPressFunction(std::function<void()> pressFunction)
{
	this->pressFunction = pressFunction;
}

void GuiButton::setUnpressFunction(std::function<void()> unpressFunction)
{
	this->unpressFunction = unpressFunction;
}

void GuiButton::setSpriteTextureBoundsFromState()
{
	int index = 0;
	if(hovered)
	{
		index += 1;
	}
	if(pressed)
	{
		index += 2;
	}
	Recti stateTextureBounds = sprite->getTextureBounds();
	stateTextureBounds.setMinKeepSize(defaultTextureBounds.min + Vector2i(defaultTextureBounds.getSize()[0] * index, 0));
	sprite->setTextureBounds(stateTextureBounds);
}


/*

Button States and Transitions

none - none
mouse over - hovered on, hoverFunction

hovered - hovered
mouse away - hovered off, unhoverFunction
mouse down - pressed on, pressFunction

pressed hovered - pressedHovered
mouse away - hovered off, unhoverFunction
timeout    - if type == shot : pressed off, unpressFunction :
mouse up   - if type == toggle : toggled on : pressed off, unpressFunction

*/

