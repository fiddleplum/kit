#include "Camera.h"
#include <exception>

namespace Scene
{
	Camera::Camera()
	{
		aspectRatio = 1.0f;
		near = 0.1f;
		far = 1000.0f;
		fov = 3.14159f / 2.0f;
		size = 1.0f;
		perspective = true;
		projection = Matrix44f::zero();
		view = Matrix44f::identity();
		projectionNeedsUpdate = true;
		viewNeedsUpdate = true;
	}

	void Camera::setAspectRatio(float newAspectRatio)
	{
		aspectRatio = newAspectRatio;
		projectionNeedsUpdate = true;
	}

	void Camera::setNear(float newNear)
	{
		near = newNear;
		projectionNeedsUpdate = true;
	}

	void Camera::setFar(float newFar)
	{
		far = newFar;
		projectionNeedsUpdate = true;
	}

	void Camera::setPerspective(float newFov)
	{
		fov = newFov;
		perspective = true;
		projectionNeedsUpdate = true;
	}

	void Camera::setOrthogonal(float newSize)
	{
		size = newSize;
		perspective = false;
		projectionNeedsUpdate = true;
	}

	void Camera::setPosition(Vector3f position)
	{
		Entity::setPosition(position);
		viewNeedsUpdate = true;
	}

	void Camera::setOrientation(Quaternionf orientation)
	{
		Entity::setOrientation(orientation);
		viewNeedsUpdate = true;
	}

	void Camera::setScale(Vector3f scale)
	{
		Entity::setScale(scale);
		viewNeedsUpdate = true;
	}

	Vector2f Camera::getNdcPosition(Vector3f positionInWorld) const
	{
		if(projectionNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateProjection();
		}
		if(viewNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateView();
		}
		return (projection * view).transform(positionInWorld, 1).shrink<2>();
	}

	Ray3f Camera::getRay(Vector2f ndcPosition) const
	{
		if(projectionNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateProjection();
		}
		if(viewNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateView();
		}
		Ray3f ray;
		ray.start = getPosition();
		Vector3f endPosition = (viewInverse * projectionInverse).transform(ndcPosition.extend<3>(-1), 1);
		ray.direction = endPosition - ray.start;
		return ray;
	}

	Matrix44f const & Camera::getProjection() const
	{
		if(projectionNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateProjection();
		}
		return projection;
	}

	Matrix44f const & Camera::getView() const
	{
		if(viewNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateView();
		}
		return view;
	}

	void Camera::updateProjection()
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
			projection(0, 0) = scaleInv;
			projection(1, 1) = scaleInv * aspectRatio;
			projectionInverse(0, 0) = scale;
			projectionInverse(1, 1) = scale / aspectRatio;
		}
		else
		{
			projection(0, 0) = scaleInv / aspectRatio;
			projection(1, 1) = scaleInv;
			projectionInverse(0, 0) = scale * aspectRatio;
			projectionInverse(1, 1) = scale;
		}
		if(perspective)
		{
			float nf2 = 2 * near * far;
			float nmf = near - far;
			float npf = near + far;
			projection(2, 2) = npf / nmf;
			projection(2, 3) = -nf2 / nmf;
			projection(3, 2) = 1;
			projection(3, 3) = 0;
			projectionInverse(2, 2) = 0;
			projectionInverse(2, 3) = -1;
			projectionInverse(3, 2) = nmf / nf2;
			projectionInverse(3, 3) = -npf / nf2;
		}
		else
		{
			// z = (z - near) / (near - far) - 1
			projection(2, 2) = 0;
			projection(2, 3) = (far - near);
			projection(3, 2) = 0;
			projection(3, 3) = 1;
			// TODO: projection and projectionInverse need to be finished
		}
		projectionNeedsUpdate = false;
	}

	void Camera::updateView()
	{
		Vector3f position = getPosition();
		Matrix33f rot = getOrientation().getMatrix();
		view(0, 0) = rot(0, 0);
		view(1, 0) = rot(0, 2);
		view(2, 0) = rot(0, 1);
		view(0, 1) = rot(1, 0);
		view(1, 1) = rot(1, 2);
		view(2, 1) = rot(1, 1);
		view(0, 2) = rot(2, 0);
		view(1, 2) = rot(2, 2);
		view(2, 2) = rot(2, 1);
		view(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0));
		view(1, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2));
		view(2, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1));
		viewInverse(0, 0) = rot(0, 0);
		viewInverse(1, 0) = rot(1, 0);
		viewInverse(2, 0) = rot(2, 0);
		viewInverse(0, 1) = rot(0, 2);
		viewInverse(1, 1) = rot(1, 2);
		viewInverse(2, 1) = rot(2, 2);
		viewInverse(0, 2) = rot(0, 1);
		viewInverse(1, 2) = rot(1, 1);
		viewInverse(2, 2) = rot(2, 1);
		viewInverse(0, 3) = position[0];
		viewInverse(1, 3) = position[1];
		viewInverse(2, 3) = position[2];
		viewNeedsUpdate = false;
	}
}

