#pragma once

#include "widget_container.h"

namespace gui
{
	class Panel : public WidgetContainer
	{
	public:
		// Gets the bounds of the panel.
		Recti getBounds() const;

		// Sets the bounds of the panel.
		void setBounds(Recti bounds);

		// Implements Widget::render.
		void render(Coord2i windowSize) const override;

	protected:
		// Updates the positions/sizes of the child widgets.
		virtual void updateWidgetBounds() = 0;

	private:
		Recti bounds;
	};
}

