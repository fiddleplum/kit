#pragma once

#include <kit/gui/widget.h>
#include <kit/ptr.h>
#include <functional>

namespace kit
{
	namespace gui
	{
		enum WidgetType { SPRITE, BUTTON };

		class WidgetContainer : virtual public Widget
		{
		public:
			// Handles container-wide events.
			virtual void setHandleContainerEventFunction (std::function<bool (Event const &, bool)>) = 0;

			// Updates the widget bounds after the container has moved or otherwise changed.
			virtual void setUpdateWidgetBoundsFunction (std::function<void ()>) = 0;

			// Gets the bounds of the container.
			virtual Recti getBounds () const = 0;

			// Sets the position of the container.
			virtual void setPosition (Vector2i position) = 0;

			// Sets the maximum size this container can be.
			virtual void setMaxSize (Vector2i maxSize) = 0;

			// Inserts a widget before another widget.
			virtual Ptr<Widget> insertWidgetBefore (WidgetType type, Ptr<Widget> beforeWidget) = 0;

			// Append a widget to the end of the list of widgets.
			virtual Ptr<Widget> addWidget (WidgetType type) = 0;

			// Remove a widget from the container.
			virtual void removeWidget (Ptr<Widget> widget) = 0;

			// Sets the position of the widget relative to this.
			virtual void setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset) = 0;

			// Sets the size of the widget relative to this.
			virtual void setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize) = 0;

			// Does the widget handle events and rendered?
			virtual bool isWidgetActive (Ptr<Widget> widget) const = 0;

			// Sets whether the widget handles events and rendered.
			virtual void setWidgetActive (Ptr<Widget> widget, bool active) = 0;

			// Is the widget rendered?
			virtual bool isWidgetVisible (Ptr<Widget> widget) const = 0;

			// Sets whether the widget is rendered.
			virtual void setWidgetVisible (Ptr<Widget> widget, bool visible) = 0;
		};
	}
}

