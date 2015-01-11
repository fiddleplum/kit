#include "Camera.h"
#include <exception>

namespace Scene2D
{
	Camera::Camera()
	{
		aspectRatio = 1.0f;
		maxViewSize = 1.0f;
		projection = projectionInverse = Matrix33f::identity();
		view = viewInverse = Matrix33f::identity();
		projectionNeedsUpdate = true;
		viewNeedsUpdate = true;
	}

	void Camera::setAspectRatio(float newAspectRatio)
	{
		aspectRatio = newAspectRatio;
		projectionNeedsUpdate = true;
	}

	void Camera::setMaxViewSize(float newMaxViewSize)
	{
		maxViewSize = newMaxViewSize;
		projectionNeedsUpdate = true;
	}

	void Camera::setPosition(Vector2f position)
	{
		Entity::setPosition(position);
		viewNeedsUpdate = true;
	}

	void Camera::setOrientation(float orientation)
	{
		Entity::setOrientation(orientation);
		viewNeedsUpdate = true;
	}

	Vector2f Camera::getAppPosition(Vector2f worldPosition) const
	{
		if(projectionNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateProjection();
		}
		if(viewNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateView();
		}
		return (projection * view).transform(worldPosition, 1);
	}

	Vector2f Camera::getWorldPosition(Vector2f appPosition) const
	{
		if(projectionNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateProjection();
		}
		if(viewNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateView();
		}
		return (viewInverse * projectionInverse).transform(appPosition, 1);
	}

	Matrix33f const & Camera::getProjection() const
	{
		if(projectionNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateProjection();
		}
		return projection;
	}

	Matrix33f const & Camera::getView() const
	{
		if(viewNeedsUpdate)
		{
			const_cast<Camera *>(this)->updateView();
		}
		return view;
	}

	void Camera::updateProjection()
	{
		if(maxViewSize == 0 || aspectRatio == 0)
		{
			throw std::exception();
		}
		float maxViewSizeInv = 1.0f / maxViewSize;
		if(aspectRatio >= 1.0f)
		{
			projection(0, 0) = maxViewSizeInv;
			projection(1, 1) = maxViewSizeInv * aspectRatio;
			projectionInverse(0, 0) = maxViewSize;
			projectionInverse(1, 1) = maxViewSize / aspectRatio;
		}
		else
		{
			projection(0, 0) = maxViewSizeInv / aspectRatio;
			projection(1, 1) = maxViewSizeInv;
			projectionInverse(0, 0) = maxViewSize * aspectRatio;
			projectionInverse(1, 1) = maxViewSize;
		}
		projectionNeedsUpdate = false;
	}

	void Camera::updateView()
	{
		Vector2f position = getPosition();
		float cosOrientation = std::cos(getOrientation());
		float sinOrientation = std::sin(getOrientation());
		view(0, 0) = cosOrientation;
		view(1, 0) = sinOrientation;
		view(0, 1) = -sinOrientation;
		view(1, 1) = cosOrientation;
		view(0, 2) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0));
		view(1, 2) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1));
		viewInverse(0, 0) = cosOrientation;
		viewInverse(1, 0) = -sinOrientation;
		viewInverse(0, 1) = sinOrientation;
		viewInverse(1, 1) = cosOrientation;
		viewInverse(0, 2) = position[0];
		viewInverse(1, 2) = position[1];
		viewNeedsUpdate = false;
	}
}

