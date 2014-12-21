#pragma once

#include "entity.h"
#include "../scene/scene.h"

namespace kit
{
	namespace flat
	{
		class Camera : public Entity
		{
		public:
			Camera(Ptr<scene::Scene> scene);

			~Camera();

			Ptr<scene::Camera> getSceneCamera() const { return _camera; }

			void setPosition(Vector2f position) override;

			void setOrientation(float orientation) override;

			float getAspectRatio() const { return _aspectRatio; }

			virtual void setAspectRatio(float aspectRatio);

			float getMaxViewSize() const { return _maxViewSize; }

			virtual void setMaxViewSize(float maxViewSize);

			Vector2f getViewSize() const { return _viewSize; }

			Vector2f getNdcPosition(Vector2f worldPosition) const;

			Vector2f getWorldPosition(Vector2f ndcPosition) const;

		private:
			void updateViewSize();

			Ptr<scene::Scene> _scene;
			Ptr<scene::Camera> _camera;
			float _aspectRatio;
			float _maxViewSize;
			Vector2f _viewSize;
		};
	}
}

