#pragma once

#include <kit/gui_element.h>
#include <kit/event.h>

class GuiElementImpl : virtual public GuiElement
{
public:
	// Can the element have focus?
	virtual bool canHaveFocus();

	// Returns true if the element has focus.
	virtual bool hasFocus() const;

	// Sets the keyboard focus.
	virtual void setFocus(bool value);

	// Handles an event with the associated window.
	// The cursorPosition is relative to the parent window.
	// If the cursor position is inside the window and not blocked by another widget, cursorPositionIsValid is true.
	// Returns true if the event is consumed.
	virtual bool handleEvent(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid);

	// Renders the element.
	virtual void render(Vector2i windowSize) const = 0;

protected:
	// Returns true if the cursor is over the element. Helper function for subclasses.
	bool cursorIsOver(Vector2i cursorPosition, bool cursorPositionIsValid) const;
};

/*
Note: Because of Viewport, I really need glScissor to be implemented in the gui container. The reason is that
the viewport can't just intersect bounds and clipBounds, or the view becomes squished, and it would be a pain
to offset and squish the camera center to work with the clipped bounds.

If gui container implements glscissor, the render part becomes much simpler. the event handling changes.
I need to put in the handleevent call if the cursor is valid or not. This goes back to what to pass
into the handleEvent function. Perhaps I need to revisit this whole issue _again_.

I like the idea of consuming the event still, because it greatly reduces the number of calls to
handleEvent on each gui element. Perhaps a small structure of the position and validity should
still be passed in, instead of the whole cursor.

Let's see: An element needs the validity and position, and the ability to change the cursor, and really
nothing else.

Let's use this:
void handleEvent(Event event, bool cursorPositionisValid, Ptr<Cursor> cursor);

Then in Sprite and even in Element, we can remove the clipBounds entirely. For button, it just needs to
check include cursorPositionIsValid in its logic.

For Element not to require clipBounds, Container should not need to know about the bounds of its parents.
The way to do this is to implement glScissorPush and pop so that each push intersects with the previous
on the scissor stack to product the new scissor. For container event handling, it still just is passed
cursorPositionIsValid, and uses its own bounds to update the variable.

This should work.

*/