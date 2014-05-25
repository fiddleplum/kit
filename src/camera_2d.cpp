#include "camera_2d.h"

namespace kit
{
	Camera2D::Camera2D()
	{
		aspectRatio = 1.0f;
		maxViewSize = 1.0f;
		viewSize.set(1.0f, 1.0f);
	}

	void Camera2D::setAspectRatio(float newAspectRatio)
	{
		aspectRatio = newAspectRatio;
		updateViewSize();
	}

	void Camera2D::setMaxViewSize(float newMaxViewSize)
	{
		maxViewSize = newMaxViewSize;
		updateViewSize();
	}

	Vector2f Camera2D::getNdcPosition(Vector2f worldPosition) const
	{
		return frame.getTransformInv().transform(worldPosition, 1).scaleInv(viewSize);
	}

	Vector2f Camera2D::getWorldPosition(Vector2f ndcPosition) const
	{
		return frame.getTransform().transform(ndcPosition.scale(viewSize), 1);
	}

	void Camera2D::updateViewSize()
	{
		if(aspectRatio > 1.0f)
		{
			viewSize.set(maxViewSize, maxViewSize / aspectRatio);
		}
		else
		{
			viewSize.set(maxViewSize * aspectRatio, maxViewSize);
		}
	}
}

