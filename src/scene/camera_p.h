#pragma once

#include "entity_p.h"
#include <kit/scene/camera.h>
#include <kit/ray.h>

namespace kit
{
	namespace scene
	{
		class CameraP : virtual public Camera, virtual public EntityP
		{
		public:
			CameraP ();

			void setPosition (Vector3f) override;

			void setOrientation (Quaternionf) override;

			void setAspectRatio (float aspectRatio) override;

			void setNear (float near) override;

			void setFar (float far) override;

			void setPerspective (float fov) override;

			void setOrthogonal (float size) override;

			Vector2f getNdcPosition (Vector3f worldPosition) const;

			Ray3f getRay (Vector2f ndcPosition) const;

			Matrix44f const & getWorldToCamera () const;

			Matrix44f const & getCameraToNdc () const;

		private:
			void updateWorldToCamera ();
			void updateCameraToNdc ();

			float aspectRatio;
			float near;
			float far;
			float fov;
			float size;
			bool perspective;
			bool cameraToNdcNeedsUpdate;
			bool worldToCameraNeedsUpdate;
			Matrix44f cameraToNdc;
			Matrix44f ndcToCamera;
			Matrix44f worldToCamera;
			Matrix44f cameraToWorld;
		};
	}
}

