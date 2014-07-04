#include "camera.h"

namespace kit
{
	Camera::Camera ()
	{
		aspectRatio = 1.0f;
		near = 0.1f;
		far = 1000.0f;
		fov = 3.14159f / 2.0f;
		size = 1.0f;
		perspective = true;
		cameraToNdcTransform = ndcToCameraTransform = Matrix44f::identity();
		worldToCameraTransform = cameraToWorldTransform = Matrix44f::identity();
		cameraToNdcTransformNeedsUpdate = true;
		worldToCameraTransformNeedsUpdate = true;
	}

	void Camera::setPosition (Vector3f position)
	{
		Entity::setPosition(position);
		worldToCameraTransformNeedsUpdate = true;
	}

	void Camera::setOrientation (Quaternionf orientation)
	{
		Entity::setOrientation(orientation);
		worldToCameraTransformNeedsUpdate = true;
	}

	void Camera::setAspectRatio (float newAspectRatio)
	{
		aspectRatio = newAspectRatio;
		cameraToNdcTransformNeedsUpdate = true;
	}

	void Camera::setNear (float newNear)
	{
		near = newNear;
		cameraToNdcTransformNeedsUpdate = true;
	}

	void Camera::setFar (float newFar)
	{
		far = newFar;
		cameraToNdcTransformNeedsUpdate = true;
	}

	void Camera::setPerspective (float newFov)
	{
		fov = newFov;
		perspective = true;
		cameraToNdcTransformNeedsUpdate = true;
	}

	void Camera::setOrthogonal (float newSize)
	{
		size = newSize;
		perspective = false;
		cameraToNdcTransformNeedsUpdate = true;
	}

	Vector2f Camera::getNdcPosition (Vector3f positionInWorld) const
	{
		if(worldToCameraTransformNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateWorldToCamera();
		}
		if(cameraToNdcTransformNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateCameraToNdc();
		}
		return (cameraToNdcTransform * worldToCameraTransform).transform(positionInWorld, 1).shrink<2>();
	}

	Ray3f Camera::getRay (Vector2f ndcPosition) const
	{
		if(worldToCameraTransformNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateWorldToCamera();
		}
		if(cameraToNdcTransformNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateCameraToNdc();
		}
		Ray3f ray;
		ray.start = getPosition();
		Vector3f endPosition = (cameraToWorldTransform * ndcToCameraTransform).transform(ndcPosition.extend<3>(-1), 1);
		ray.direction = endPosition - ray.start;
		return ray;
	}

	Matrix44f const & Camera::getWorldToCameraTransform () const
	{
		if(worldToCameraTransformNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateWorldToCamera();
		}
		return worldToCameraTransform;
	}

	Matrix44f const & Camera::getCameraToNdcTransform () const
	{
		if(cameraToNdcTransformNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateCameraToNdc();
		}
		return cameraToNdcTransform;
	}

	void Camera::updateWorldToCamera ()
	{
		Vector3f position = getPosition();
		Matrix33f rot = getOrientation().getMatrix();
		worldToCameraTransform(0, 0) = rot(0, 0);
		worldToCameraTransform(1, 0) = rot(0, 2);
		worldToCameraTransform(2, 0) = rot(0, 1);
		worldToCameraTransform(0, 1) = rot(1, 0);
		worldToCameraTransform(1, 1) = rot(1, 2);
		worldToCameraTransform(2, 1) = rot(1, 1);
		worldToCameraTransform(0, 2) = rot(2, 0);
		worldToCameraTransform(1, 2) = rot(2, 2);
		worldToCameraTransform(2, 2) = rot(2, 1);
		worldToCameraTransform(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0));
		worldToCameraTransform(1, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2));
		worldToCameraTransform(2, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1));
		cameraToWorldTransform(0, 0) = rot(0, 0);
		cameraToWorldTransform(1, 0) = rot(1, 0);
		cameraToWorldTransform(2, 0) = rot(2, 0);
		cameraToWorldTransform(0, 1) = rot(0, 2);
		cameraToWorldTransform(1, 1) = rot(1, 2);
		cameraToWorldTransform(2, 1) = rot(2, 2);
		cameraToWorldTransform(0, 2) = rot(0, 1);
		cameraToWorldTransform(1, 2) = rot(1, 1);
		cameraToWorldTransform(2, 2) = rot(2, 1);
		cameraToWorldTransform(0, 3) = position[0];
		cameraToWorldTransform(1, 3) = position[1];
		cameraToWorldTransform(2, 3) = position[2];
		worldToCameraTransformNeedsUpdate = false;
	}

	void Camera::updateCameraToNdc ()
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
			cameraToNdcTransform(0, 0) = scaleInv;
			cameraToNdcTransform(1, 1) = scaleInv * aspectRatio;
			ndcToCameraTransform(0, 0) = scale;
			ndcToCameraTransform(1, 1) = scale / aspectRatio;
		}
		else
		{
			cameraToNdcTransform(0, 0) = scaleInv / aspectRatio;
			cameraToNdcTransform(1, 1) = scaleInv;
			ndcToCameraTransform(0, 0) = scale * aspectRatio;
			ndcToCameraTransform(1, 1) = scale;
		}
		if(perspective)
		{
			float nf2 = 2 * near * far;
			float nmf = near - far;
			float npf = near + far;
			cameraToNdcTransform(2, 2) = npf / nmf;
			cameraToNdcTransform(2, 3) = -nf2 / nmf;
			cameraToNdcTransform(3, 2) = 1;
			cameraToNdcTransform(3, 3) = 0;
			ndcToCameraTransform(2, 2) = 0;
			ndcToCameraTransform(2, 3) = -1;
			ndcToCameraTransform(3, 2) = nmf / nf2;
			ndcToCameraTransform(3, 3) = -npf / nf2;
		}
		else
		{
			float nmf = near - far;
			float npf = near + far;
			cameraToNdcTransform(2, 2) = 2 / nmf;
			cameraToNdcTransform(2, 3) = -npf / nmf;
			cameraToNdcTransform(3, 2) = 0;
			cameraToNdcTransform(3, 3) = 1;
			ndcToCameraTransform(2, 2) = nmf / 2;
			ndcToCameraTransform(2, 3) = npf / 2;
			ndcToCameraTransform(3, 2) = 0;
			ndcToCameraTransform(3, 3) = 1;
		}
		cameraToNdcTransformNeedsUpdate = false;
	}
}

