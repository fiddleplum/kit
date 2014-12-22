#pragma once

#include "scene_camera.h"
#include "scene.h"
#include "gui_element.h"

class GuiViewport : public GuiElement
{
public:
	GuiViewport();

	// Implements parent.
	Recti getBounds() const override;

	// Implements parent.
	void setPosition(Vector2i position) override;

	// Implements parent.
	void render(Vector2i windowSize) const override;

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

