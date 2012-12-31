#pragma once

#include "Vector.h"
#include <vector>
#include <string>

//! Sets the title of the application.
void setTitle (std::string const & text);

//! Quits the application.
void quit ();

//! Toggle the application between windowed and fullscreen.
void toggleFullScreen ();

// The user must implement these functions.

//! When the application has initialized, this fucntion is called.
void onInitialized (std::vector<std::string> const & params);

//! When the user tries to close the application, this function is called.
void onClose ();

//! When the application is resized, this function is called.
void onResized (Vector2i newSize);

//! When the application received input, this function is called.
void processInput ();

//! When the application needs to tick the content, this function is called.
void tick (float timeSinceLastTick);

//! When the application needs to render the content, this function is called.
void render ();

