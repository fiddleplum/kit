#include "panel.h"
#include "../open_gl.h"

namespace gui
{
	Recti Panel::getBounds() const
	{
		return bounds;
	}

	void Panel::setBounds(Recti _bounds)
	{
		bounds = _bounds;
		updateWidgetBounds();
	}

	void Panel::render(Vector2i windowSize) const
	{
		glScissorPush(bounds.min[0], bounds.min[1], bounds.getSize()[0], bounds.getSize()[1]);
		WidgetContainer::render(windowSize);
		glScissorPop();
	}
}

