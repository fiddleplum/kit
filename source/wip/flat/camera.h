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

			void setPosition(Coord2f position) override;

			void setOrientation(float orientation) override;

			float getAspectRatio() const { return _aspectRatio; }

			virtual void setAspectRatio(float aspectRatio);

			float getMaxViewSize() const { return _maxViewSize; }

			virtual void setMaxViewSize(float maxViewSize);

			Coord2f getViewSize() const { return _viewSize; }

			Coord2f getNdcPosition(Coord2f worldPosition) const;

			Coord2f getWorldPosition(Coord2f ndcPosition) const;

		private:
			void updateViewSize();

			Ptr<scene::Scene> _scene;
			Ptr<scene::Camera> _camera;
			float _aspectRatio;
			float _maxViewSize;
			Coord2f _viewSize;
		};
	}
}

