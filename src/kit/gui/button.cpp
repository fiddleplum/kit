#include "../app.h"
#include "../resources.h"
#include "button.h"
#include "sprite.h"

namespace kit
{
	namespace gui
	{
		Button::Button()
		{
			sprite.setNew();
			type = Hold;
			selected = false;
			hovered = false;
			pressed = false;
			toggled = false;
			shotInterval = .5f;
		}

		Recti Button::getBounds() const
		{
			return sprite->getBounds();
		}

		void Button::setPosition(Vector2i position)
		{
			sprite->setPosition(position);
		}

		void Button::setMaxSize(Vector2i maxSize)
		{
			sprite->setMaxSize(maxSize);
		}

		void Button::handleEvent(Event const & event)
		{
			bool oldPressedOrToggled = pressed || toggled;

			switch(event.type)
			{
				case Event::Update:
				{
					if(type == Shot && pressed && app::getTime() - timePressed > shotInterval)
					{
						pressed = false;
					}
					Ptr<Cursor> cursor = event.window->getCursor();
					if(event.window->getCursor()->isPositionValid() && getBounds().containsEx(cursor->getPosition()))
					{
						if(hovered == false)
						{
							hovered = true;
							if(hoverFunction != nullptr)
							{
								hoverFunction();
							}
						}
					}
					else
					{
						if(hovered == true)
						{
							hovered = false;
							pressed = false;
							if(unhoverFunction != nullptr)
							{
								unhoverFunction();
							}
						}
					}
					break;
				}
				case Event::Keyboard:
				{
					KeyboardEvent const & kEvent = event.as<KeyboardEvent>();
					if(selected)
					{
						if(kEvent.key == KeyboardEvent::Enter)
						{
							if(kEvent.pressed)
							{
								press();
							}
							else
							{
								pressed = false;
							}
						}
					}
					break;
				}
				case Event::MouseButton:
				{
					MouseButtonEvent const & mbEvent = event.as<MouseButtonEvent>();
					if(mbEvent.button == mbEvent.Left)
					{
						if(mbEvent.pressed)
						{
							if(hovered)
							{
								press();
							}
						}
						else
						{
							pressed = false;
						}
					}
					break;
				}
				case kit::Event::ControllerButton:
				{
					auto cbe = event.as<kit::ControllerButtonEvent>();
					if(selected && cbe.pressed)
					{
						press();
					}
					break;
				}
				default:
					break;
			};
			if(!oldPressedOrToggled && (pressed || toggled))
			{
				if(pressFunction != nullptr)
				{
					pressFunction();
				}
			}
			if(oldPressedOrToggled && !(pressed || toggled))
			{
				if(unpressFunction != nullptr)
				{
					unpressFunction();
				}
			}
			setSpriteTextureBoundsFromState();
			sprite->handleEvent(event);
		}

		void Button::render(Vector2i windowSize)
		{
			sprite->render(windowSize);
		}

		Ptr<Texture> Button::getTexture() const
		{
			return sprite->getTexture();
		}

		void Button::setTexture(Ptr<Texture> texture)
		{
			sprite->setTexture(texture);
		}

		void Button::setTexture(std::string const & filename)
		{
			setTexture(resources::getTextureFromFile(filename));
		}

		void Button::setTextureBounds(Recti bounds)
		{
			defaultTextureBounds = bounds;
			sprite->setTextureBounds(bounds);
		}

		void Button::setType(Type type)
		{
			this->type = type;
		}

		void Button::setShotInterval(float interval)
		{
			shotInterval = interval;
		}

		void Button::setSelected(bool _selected)
		{
			selected = _selected;
		}

		void Button::setHoverFunction(std::function<void()> hoverFunction)
		{
			this->hoverFunction = hoverFunction;
		}

		void Button::setUnhoverFunction(std::function<void()> unhoverFunction)
		{
			this->unhoverFunction = unhoverFunction;
		}

		void Button::setPressFunction(std::function<void()> pressFunction)
		{
			this->pressFunction = pressFunction;
		}

		void Button::setUnpressFunction(std::function<void()> unpressFunction)
		{
			this->unpressFunction = unpressFunction;
		}

		void Button::press()
		{
			pressed = true;
			if(type == Toggle)
			{
				toggled = !toggled;
			}
			if(type == Shot)
			{
				timePressed = app::getTime();
			}
		}

		void Button::setSpriteTextureBoundsFromState()
		{
			int index = 0;
			if(hovered || selected)
			{
				index += 1;
			}
			if(pressed || toggled)
			{
				index += 2;
			}
			Recti stateTextureBounds = sprite->getTextureBounds();
			stateTextureBounds.setMinKeepSize(defaultTextureBounds.min + Vector2i(defaultTextureBounds.getSize()[0] * index, 0));
			sprite->setTextureBounds(stateTextureBounds);
		}
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
}

