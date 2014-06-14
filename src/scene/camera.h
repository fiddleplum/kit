#pragma once

#include "frame.h"
#include <kit/scene/camera.h>
#include <kit/ray.h>

namespace kit
{
	namespace internal
	{
		namespace scene
		{
			class Camera : virtual public kit::scene::Camera
			{
			public:
				Camera ();

				Vector3f const & getPosition () const override;

				void setPosition (Vector3f) override;

				Quaternionf const & getOrientation () const override;

				void setOrientation (Quaternionf) override;

				void setAspectRatio (float aspectRatio) override;

				void setNear (float near) override;

				void setFar (float far) override;

				void setPerspective (float fov) override;

				void setOrthogonal (float size) override;

				Vector2f getNdcPosition (Vector3f worldPosition) const;

				Ray3f getRay (Vector2f ndcPosition) const;

				Matrix44f const & getProjection () const;

				Matrix44f const & getView () const;

			private:
				void updateProjection ();
				void updateView ();

				Frame frame;
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
}

