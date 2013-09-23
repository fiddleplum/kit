#pragma once

#include "App.h"
#include "Vector.h"
#include <string>

namespace App
{
	/// Returns the number of controllers.
	int getNumControllers();

	/// Reloads the controllers, enabling plug-in and play functionality.
	void refreshControllers();

	/// Returns true if the cursor has a valid position (not disabled and within the window).
	bool hasValidCursorPosition();
	
	/// Returns the cursor position. If the cursor does not have a valid position, it fails an assertion.
	Vector2i getCursorPosition();
	
	/// Returns true if the system cursor is visible.
	bool isCursorVisible();
	
	/// Sets the system cursor is visible. This can be used to make a custom OpenGL cursor.
	void setCursorVisible(bool visible);
	
	/// Returns true if the cursor is enabled.
	bool isCursorEnabled();
	
	/// Enables or disables the cursor. When disabled, the mouse will always return good x and y offsets, which is good for first-person views or 'grabbing' 3D objects. May throw runtime_exception.
	void setCursorEnabled(bool enabled);
}

