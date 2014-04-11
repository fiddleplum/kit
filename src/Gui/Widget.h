#pragma once

#include "../Vector.h"
#include "../Interval.h"
#include "../Event.h"

namespace App
{
	void handleEvent(Event const & event);
	void render();
}

namespace app
{
	class Widget
	{
	public:
		Widget() {} // Empty default constructor.
		virtual ~Widget() {} // Virtual destructor for subclasses.

		virtual Recti getBounds() const = 0; // Gets the bounds of the widget.
		virtual void setPosition(Vector2i position) = 0; // Sets the position of the widget.
		virtual void setMaxSize(Vector2i maxSize) = 0; // Sets the maximum size this widget can be. The widget can max out to this size (text box), or not change its size at all (check box).

	protected:
		virtual void handleEvent(Event const & event) = 0; // Handles events.
		virtual void render() = 0; // Renders the widget.

	private:
		// TODO: Make these public and with the delete specifier, once MSVC upgrades.
		/// Not copy-constructable.
		Widget(Widget const &);

		/// Not copyable.
		Widget & operator = (Widget const &);

		friend void App::handleEvent(Event const & event);
		friend void App::render();
		friend class WidgetContainer;
	};
}

