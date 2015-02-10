#pragma once

#include "event.h"
#include "rect.h"

class GuiElement
{
public:
	// Virtual destructor, so it can be deleted properly.
	virtual ~GuiElement() {}

	// Returns the bounds of the element.
	virtual Recti getBounds() const = 0;

	// Sets the position of the element.
	virtual void setPosition(Coord2i position) = 0;

	// Sets the size of the element. Note that some elements will not change their size.
	virtual void setSize(Coord2i size) = 0;

	// Can the element have focus?
	virtual bool canHaveFocus();

	// Returns true if the element has focus.
	virtual bool hasFocus() const;

	// Sets the keyboard focus.
	virtual void setFocus(bool value);

	// Called by GuiContainer to handle an event.
	// The cursorPosition is relative to the parent window.
	// If the cursor position is inside the window and not blocked by another widget, cursorPositionIsValid is true.
	// Returns true if the event is consumed.
	virtual bool handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid);

	// Called by GuiContainer to update the element every frame.
	virtual void update(float dt) {}

	// Called by GuiContainer to update the element after a regular update but before the render. Good for things that keep track of scene elements.
	virtual void preRenderUpdate() {}

	// Called by GuiContainer to render the element.
	virtual void render(Coord2i windowSize) const = 0;

protected:
	// Returns true if the cursor is within the bounds of the element. Helper function for subclasses.
	bool cursorIsOver(Coord2i cursorPosition, bool cursorPositionIsValid) const;
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