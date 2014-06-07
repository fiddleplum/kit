#include "button_internal.h"
#include "../app_internal.h"

namespace kit
{
	namespace gui
	{
		ButtonInternal::ButtonInternal ()
		{
			sprite.set(new SpriteInternal);
			type = Hold;
			hovered = false;
			pressed = false;
			toggled = false;
			shotInterval = .5f;
		}

		Recti ButtonInternal::getBounds () const
		{
			return sprite->getBounds();
		}

		void ButtonInternal::setPosition (Vector2i position)
		{
			sprite->setPosition(position);
		}

		void ButtonInternal::setMaxSize (Vector2i maxSize)
		{
			sprite->setMaxSize(maxSize);
		}

		bool ButtonInternal::handleEvent (Event const & event, bool cursorIsValid)
		{
			bool oldPressedOrToggled = pressed || toggled;

			if(cursorIsValid)
			{
				if(getBounds().containsEx(app()->getCursorPosition()))
				{
					cursorIsValid = false;
				}
			}

			switch(event.type)
			{
				case Event::Update:
				{
					if(type == Shot && pressed && app()->getTime() - timePressed > shotInterval)
					{
						pressed = false;
					}
					break;
				}
				case Event::MouseMove:
				{
					MouseMoveEvent const & mmEvent = event.as<MouseMoveEvent>();
					if(getBounds().containsInc(mmEvent.absolute))
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
				case Event::MouseButton:
				{
					MouseButtonEvent const & mbEvent = event.as<MouseButtonEvent>();
					if(mbEvent.button == mbEvent.Left)
					{
						if(mbEvent.pressed)
						{
							if(hovered)
							{
								pressed = true;
								if(type == Toggle)
								{
									toggled = !toggled;
								}
								if(type == Shot)
								{
									timePressed = app()->getTime();
								}
							}
						}
						else
						{
							pressed = false;
						}
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
			return cursorIsValid;
		}

		void ButtonInternal::render (Vector2i windowSize)
		{
			sprite->render(windowSize);
		}

		void ButtonInternal::setTexture (std::string const & filename)
		{
			sprite->setTexture(filename);
		}

		void ButtonInternal::setTextureBounds (Recti bounds)
		{
			defaultTextureBounds = bounds;
			sprite->setTextureBounds(bounds);
		}

		void ButtonInternal::setType (Type type)
		{
			this->type = type;
		}

		void ButtonInternal::setShotInterval (float interval)
		{
			shotInterval = interval;
		}

		void ButtonInternal::setHoverFunction (std::function<void ()> hoverFunction)
		{
			this->hoverFunction = hoverFunction;
		}

		void ButtonInternal::setUnhoverFunction (std::function<void ()> unhoverFunction)
		{
			this->unhoverFunction = unhoverFunction;
		}

		void ButtonInternal::setPressFunction (std::function<void ()> pressFunction)
		{
			this->pressFunction = pressFunction;
		}

		void ButtonInternal::setUnpressFunction (std::function<void ()> unpressFunction)
		{
			this->unpressFunction = unpressFunction;
		}

		bool ButtonInternal::isHovered () const
		{
			return hovered;
		}

		bool ButtonInternal::isPressed () const
		{
			return pressed;
		}

		void ButtonInternal::setSpriteTextureBoundsFromState ()
		{
			int index = 0;
			if(hovered)
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

