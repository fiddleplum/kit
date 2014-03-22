#pragma once

#include "Widget.h"
#include <memory>

namespace Scene
{
	class Camera;
	class Scene;
}

namespace Gui
{
	class Viewport : public Widget
	{
	public:
		Viewport();

		// Inherited from Widget.
		Recti getBounds() const override;
		void setPosition(Vector2i position) override;
		void setMaxSize(Vector2i maxSize) override;
		void handleEvent(Event const & event) override;
		void render() override;

		// You can attach a camera.
		std::shared_ptr<Scene::Camera> getCamera() const;
		void setCamera(std::shared_ptr<Scene::Camera>);

		// You can attach a scene.
		std::shared_ptr<Scene::Scene> getScene() const;
		void setScene(std::shared_ptr<Scene::Scene>);

	private:
		Recti bounds;
		std::shared_ptr<Scene::Camera> camera;
		std::shared_ptr<Scene::Scene> scene;
	};
}

