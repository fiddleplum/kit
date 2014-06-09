#pragma once

#include "entity_internal.h"
#include <kit/scene/camera.h>
#include <kit/ray.h>

namespace kit
{
	namespace scene
	{
		class CameraInternal : virtual public Camera, virtual public EntityInternal
		{
		public:
			CameraInternal ();

			void setAspectRatio (float aspectRatio) override;

			void setNear (float near) override;

			void setFar (float far) override;

			void setPerspective (float fov) override;

			void setOrthogonal (float size) override;

			void setPosition (Vector3f) override;

			void setOrientation (Quaternionf) override;

			Vector2f getNdcPosition (Vector3f worldPosition) const;

			Ray3f getRay (Vector2f ndcPosition) const;

			Matrix44f const & getProjection () const;

			Matrix44f const & getView () const;

		private:
			void updateProjection ();
			void updateView ();

			float aspectRatio;
			float near;
			float far;
			float fov;
			float size;
			bool perspective;
			bool projectionNeedsUpdate;
			bool viewNeedsUpdate;
			Matrix44f projection;
			Matrix44f projectionInverse;
			Matrix44f view;
			Matrix44f viewInverse;
		};
	}
}

