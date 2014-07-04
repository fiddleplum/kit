#include "camera.h"

namespace kit
{
	namespace flat
	{
		Camera::Camera(Ptr<Scene> scene)
		{
			_scene = scene;
			_aspectRatio = 1.0f;
			_maxViewSize = 1.0f;
			_viewSize.set(1.0f, 1.0f);
			_camera = scene->addCamera();
			_camera->setOrthogonal(1.0f);
			_camera->setNear(1.0f);
			_camera->setFar(1000.0f);
			_camera->setPosition(Vector3f(0, 0, 999.0f));
			_camera->setOrientation(Quaternionf(0, -3.14159265359f / 2.0f, 0)); // looking straight down
		}

		Camera::~Camera()
		{
			_scene->removeCamera(_camera);
		}

		void Camera::setAspectRatio(float aspectRatio)
		{
			_aspectRatio = aspectRatio;
			_camera->setAspectRatio(_aspectRatio);
			updateViewSize();
		}

		void Camera::setMaxViewSize(float maxViewSize)
		{
			_maxViewSize = maxViewSize;
			_camera->setOrthogonal(maxViewSize);
			updateViewSize();
		}

		Vector2f Camera::getNdcPosition(Vector2f worldPosition) const
		{
			return getTransformInv().transform(worldPosition, 1).scaleInv(_viewSize);
		}

		Vector2f Camera::getWorldPosition(Vector2f ndcPosition) const
		{
			return getTransform().transform(ndcPosition.scale(_viewSize), 1);
		}

		void Camera::updateViewSize()
		{
			if(_aspectRatio > 1.0f)
			{
				_viewSize.set(_maxViewSize, _maxViewSize / _aspectRatio);
			}
			else
			{
				_viewSize.set(_maxViewSize * _aspectRatio, _maxViewSize);
			}
		}
	}
}

