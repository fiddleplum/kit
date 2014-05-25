#pragma once

#include "widget.h"
#include <memory>

namespace kit
{
	class Camera;
	class Scene;

	namespace app
	{
		class View : public Widget
		{
		public:
			View();

			// Inherited from Widget.
			Recti getBounds() const override;
			void setPosition(Vector2i position) override;
			void setMaxSize(Vector2i maxSize) override;
			void handleEvent(Event const & event) override;
			void render() override;

			// You can attach a camera.
			std::shared_ptr<Camera> getCamera() const;
			void setCamera(std::shared_ptr<Camera>);

			// You can attach a scene.
			std::shared_ptr<Scene> getScene() const;
			void setScene(std::shared_ptr<Scene>);

		private:
			Recti bounds;
			std::shared_ptr<Camera> camera;
			std::shared_ptr<Scene> scene;
		};
	}
}

