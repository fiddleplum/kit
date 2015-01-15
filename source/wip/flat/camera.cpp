#include "camera.h"
#include "../math_util.h"

namespace kit
{
	namespace flat
	{
		Camera::Camera(Ptr<scene::Scene> scene)
		{
			if(!scene.isValid())
			{
				throw nullptr_exception();
			}
			_scene = scene;
			_aspectRatio = 1.0f;
			_maxViewSize = 1.0f;
			_viewSize.set(1.0f, 1.0f);
			_camera = scene->addCamera();
			_camera->setOrthogonal(1.0f);
			_camera->setNear(1.0f);
			_camera->setFar(1000.0f);
			_camera->setPosition(Coord3f(0, 0, 999.0f));
			_camera->setOrientation(Quaternionf(0, -(float)math::PI_OVER_2, 0)); // looking straight down
		}

		Camera::~Camera()
		{
			if(_scene.isValid())
			{
				_scene->removeCamera(_camera);
			}
		}

		void Camera::setPosition(Coord2f position)
		{
			Entity::setPosition(position);
			if(_camera.isValid())
			{
				_camera->setPosition(position.extend<3>(999.0f));
			}
		}

		void Camera::setOrientation(float orientation)
		{
			Entity::setOrientation(orientation);
			if(_camera.isValid())
			{
				_camera->setOrientation(Quaternionf(orientation, -(float)math::PI_OVER_2, 0));
			}
		}

		void Camera::setAspectRatio(float aspectRatio)
		{
			_aspectRatio = aspectRatio;
			if(_camera.isValid())
			{
				_camera->setAspectRatio(_aspectRatio);
			}
			updateViewSize();
		}

		void Camera::setMaxViewSize(float maxViewSize)
		{
			_maxViewSize = maxViewSize;
			if(_camera.isValid())
			{
				_camera->setOrthogonal(maxViewSize);
			}
			updateViewSize();
		}

		Coord2f Camera::getNdcPosition(Coord2f worldPosition) const
		{
			return getTransformInv().transform(worldPosition, 1).scaleInv(_viewSize);
		}

		Coord2f Camera::getWorldPosition(Coord2f ndcPosition) const
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

