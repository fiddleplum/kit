#pragma once

#include "entity.h"
#include "../ray.h"

namespace kit
{
	namespace scene
	{
		class Camera : public Entity
		{
		public:
			Camera ();

			void setPosition(Vector3f position) override;

			void setOrientation(Quaternionf orientation) override;

			void setAspectRatio(float aspectRatio);

			void setNear(float near);

			void setFar(float far);

			void setPerspective(float fov);

			void setOrthogonal(float size);

			Vector2f getNdcPosition(Vector3f worldPosition) const;

			Ray3f getRay(Vector2f ndcPosition) const;

			Matrix44f const & getWorldToCameraTransform() const;

			Matrix44f const & getCameraToNdcTransform() const;

		private:
			void updateWorldToCamera ();
			void updateCameraToNdc ();

			float aspectRatio;
			float near;
			float far;
			float fov;
			float size;
			bool perspective;
			bool cameraToNdcTransformNeedsUpdate;
			bool worldToCameraTransformNeedsUpdate;
			Matrix44f cameraToNdcTransform;
			Matrix44f ndcToCameraTransform;
			Matrix44f worldToCameraTransform;
			Matrix44f cameraToWorldTransform;
		};
	}
}

