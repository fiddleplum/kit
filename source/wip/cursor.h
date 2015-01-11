#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <map>

class Window;

class Cursor
{
public:
	// Constructor.
	Cursor();

	// Gets the cursor position relative to the window.
	Vector2i getPosition();

	// Returns true if the cursor position is within the window and active.
	bool isPositionValid();

	// Returns if the cursor is active.
	bool isActive();

	// Sets the cursor as active or inactive. If inactive, the cursor will not "hit" the edge of the screen (good for FPS games). This affects all windows.
	void setActive(bool state);

	// Called by Window to set the position.
	void setPosition(Vector2i position);

	// Called by Window to set the within window state.
	void setWithinWindow(bool state);

private:
	Vector2i position;
	bool withinWindow;
	static bool active;

	friend class Window;
};

