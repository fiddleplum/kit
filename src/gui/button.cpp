#include "button_p.h"
#include "../app_p.h"

namespace kit
{
	namespace gui
	{
		ButtonP::ButtonP ()
		{
			sprite.set(new SpriteP);
			type = Hold;
			hovered = false;
			pressed = false;
			toggled = false;
			shotInterval = .5f;
		}

		Recti ButtonP::getBounds () const
		{
			return sprite->getBounds();
		}

		void ButtonP::setPosition (Vector2i position)
		{
			sprite->setPosition(position);
		}

		void ButtonP::setMaxSize (Vector2i maxSize)
		{
			sprite->setMaxSize(maxSize);
		}

		bool ButtonP::handleEvent (Event const & event, bool cursorIsValid)
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

		void ButtonP::render (Vector2i windowSize)
		{
			sprite->render(windowSize);
		}

		void ButtonP::setTexture (std::string const & filename)
		{
			sprite->setTexture(filename);
		}

		void ButtonP::setTextureBounds (Recti bounds)
		{
			defaultTextureBounds = bounds;
			sprite->setTextureBounds(bounds);
		}

		void ButtonP::setType (Type type)
		{
			this->type = type;
		}

		void ButtonP::setShotInterval (float interval)
		{
			shotInterval = interval;
		}

		void ButtonP::setHoverFunction (std::function<void ()> hoverFunction)
		{
			this->hoverFunction = hoverFunction;
		}

		void ButtonP::setUnhoverFunction (std::function<void ()> unhoverFunction)
		{
			this->unhoverFunction = unhoverFunction;
		}

		void ButtonP::setPressFunction (std::function<void ()> pressFunction)
		{
			this->pressFunction = pressFunction;
		}

		void ButtonP::setUnpressFunction (std::function<void ()> unpressFunction)
		{
			this->unpressFunction = unpressFunction;
		}

		void ButtonP::setSpriteTextureBoundsFromState ()
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

