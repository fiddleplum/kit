#include "camera_p.h"

namespace kit
{
	namespace scene
	{
		CameraP::CameraP ()
		{
			aspectRatio = 1.0f;
			near = 0.1f;
			far = 1000.0f;
			fov = 3.14159f / 2.0f;
			size = 1.0f;
			perspective = true;
			cameraToNdc = ndcToCamera = Matrix44f::zero();
			worldToCamera = cameraToWorld = Matrix44f::identity();
			cameraToNdcNeedsUpdate = true;
			worldToCameraNeedsUpdate = true;
		}

		void CameraP::setPosition (Vector3f position)
		{
			EntityP::setPosition(position);
			worldToCameraNeedsUpdate = true;
		}

		void CameraP::setOrientation (Quaternionf orientation)
		{
			EntityP::setOrientation(orientation);
			worldToCameraNeedsUpdate = true;
		}

		void CameraP::setAspectRatio (float newAspectRatio)
		{
			aspectRatio = newAspectRatio;
			cameraToNdcNeedsUpdate = true;
		}

		void CameraP::setNear (float newNear)
		{
			near = newNear;
			cameraToNdcNeedsUpdate = true;
		}

		void CameraP::setFar (float newFar)
		{
			far = newFar;
			cameraToNdcNeedsUpdate = true;
		}

		void CameraP::setPerspective (float newFov)
		{
			fov = newFov;
			perspective = true;
			cameraToNdcNeedsUpdate = true;
		}

		void CameraP::setOrthogonal (float newSize)
		{
			size = newSize;
			perspective = false;
			cameraToNdcNeedsUpdate = true;
		}

		Vector2f CameraP::getNdcPosition (Vector3f positionInWorld) const
		{
			if(worldToCameraNeedsUpdate)
			{
				const_cast<CameraP *>(this)->updateWorldToCamera();
			}
			if(cameraToNdcNeedsUpdate)
			{
				const_cast<CameraP *>(this)->updateCameraToNdc();
			}
			return (cameraToNdc * worldToCamera).transform(positionInWorld, 1).shrink<2>();
		}

		Ray3f CameraP::getRay (Vector2f ndcPosition) const
		{
			if(worldToCameraNeedsUpdate)
			{
				const_cast<CameraP *>(this)->updateWorldToCamera();
			}
			if(cameraToNdcNeedsUpdate)
			{
				const_cast<CameraP *>(this)->updateCameraToNdc();
			}
			Ray3f ray;
			ray.start = getPosition();
			Vector3f endPosition = (cameraToWorld * ndcToCamera).transform(ndcPosition.extend<3>(-1), 1);
			ray.direction = endPosition - ray.start;
			return ray;
		}

		Matrix44f const & CameraP::getWorldToCamera () const
		{
			if(worldToCameraNeedsUpdate)
			{
				const_cast<CameraP *>(this)->updateWorldToCamera();
			}
			return worldToCamera;
		}

		Matrix44f const & CameraP::getCameraToNdc () const
		{
			if(cameraToNdcNeedsUpdate)
			{
				const_cast<CameraP *>(this)->updateCameraToNdc();
			}
			return cameraToNdc;
		}

		void CameraP::updateWorldToCamera ()
		{
			Vector3f position = getPosition();
			Matrix33f rot = getOrientation().getMatrix();
			worldToCamera(0, 0) = rot(0, 0);
			worldToCamera(1, 0) = rot(0, 2);
			worldToCamera(2, 0) = rot(0, 1);
			worldToCamera(0, 1) = rot(1, 0);
			worldToCamera(1, 1) = rot(1, 2);
			worldToCamera(2, 1) = rot(1, 1);
			worldToCamera(0, 2) = rot(2, 0);
			worldToCamera(1, 2) = rot(2, 2);
			worldToCamera(2, 2) = rot(2, 1);
			worldToCamera(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0));
			worldToCamera(1, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2));
			worldToCamera(2, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1));
			cameraToWorld(0, 0) = rot(0, 0);
			cameraToWorld(1, 0) = rot(1, 0);
			cameraToWorld(2, 0) = rot(2, 0);
			cameraToWorld(0, 1) = rot(0, 2);
			cameraToWorld(1, 1) = rot(1, 2);
			cameraToWorld(2, 1) = rot(2, 2);
			cameraToWorld(0, 2) = rot(0, 1);
			cameraToWorld(1, 2) = rot(1, 1);
			cameraToWorld(2, 2) = rot(2, 1);
			cameraToWorld(0, 3) = position[0];
			cameraToWorld(1, 3) = position[1];
			cameraToWorld(2, 3) = position[2];
			worldToCameraNeedsUpdate = false;
		}

		void CameraP::updateCameraToNdc ()
		{
			float scale;
			if(perspective)
			{
				scale = std::tan(fov / 2.0f);
			}
			else
			{
				scale = size;
			}
			if(scale == 0 || aspectRatio == 0 || far == near || near == 0 || far == 0)
			{
				throw std::exception();
			}
			float scaleInv = 1.0f / scale;
			if(aspectRatio >= 1.0f)
			{
				cameraToNdc(0, 0) = scaleInv;
				cameraToNdc(1, 1) = scaleInv * aspectRatio;
				ndcToCamera(0, 0) = scale;
				ndcToCamera(1, 1) = scale / aspectRatio;
			}
			else
			{
				cameraToNdc(0, 0) = scaleInv / aspectRatio;
				cameraToNdc(1, 1) = scaleInv;
				ndcToCamera(0, 0) = scale * aspectRatio;
				ndcToCamera(1, 1) = scale;
			}
			if(perspective)
			{
				float nf2 = 2 * near * far;
				float nmf = near - far;
				float npf = near + far;
				cameraToNdc(2, 2) = npf / nmf;
				cameraToNdc(2, 3) = -nf2 / nmf;
				cameraToNdc(3, 2) = 1;
				cameraToNdc(3, 3) = 0;
				ndcToCamera(2, 2) = 0;
				ndcToCamera(2, 3) = -1;
				ndcToCamera(3, 2) = nmf / nf2;
				ndcToCamera(3, 3) = -npf / nf2;
			}
			else
			{
				float nmf = near - far;
				float npf = near + far;
				cameraToNdc(2, 2) = 2 * nmf;
				cameraToNdc(2, 3) = npf / nmf;
				cameraToNdc(3, 2) = 0;
				cameraToNdc(3, 3) = 1;
				ndcToCamera(2, 2) = nmf / 2;
				ndcToCamera(2, 3) = npf / 2;
				ndcToCamera(3, 2) = 0;
				ndcToCamera(3, 3) = 1;
			}
			cameraToNdcNeedsUpdate = false;
		}
	}
}

