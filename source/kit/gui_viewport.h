#pragma once

#include "scene_camera.h"
#include "scene.h"
#include "gui_element.h"

class GuiViewport : public GuiElement
{
public:
	// Constructor.
	GuiViewport();

	// Implements parent.
	Recti getBounds() const override;

	// Implements parent.
	void setPosition(Coord2i position) override;

	// Implements parent.
	void setSize(Coord2i size) override;

	// Implements parent.
	void render(Coord2i windowSize) const override;

	// Get the attached camera.
	Ptr<SceneCamera> getCamera() const;

	// Attach a camera.
	void setCamera(Ptr<SceneCamera>);

	// Get the attached scene.
	Ptr<Scene> getScene() const;

	// Attach a scene.
	void setScene(Ptr<Scene>);

private:
	Recti bounds;
	Recti clipBounds;
	Ptr<SceneCamera> camera;
	Ptr<Scene> scene;
};

