#include "Camera.h"
#include "Scene.h"
#include <exception>

Camera::Camera(Scene * newScene)
{
	scene = newScene;
	aspectRatio = 1.0f;
	near = 0.1f;
	far = 1000.0f;
	fov = 1.0f;
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

Frame const & Camera::getFrame() const
{
	return frame;
}

void Camera::setFrame(Frame const & newFrame)
{
	frame = newFrame;
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
	ray.start = frame.getPosition();
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

void Camera::render()
{
	scene->render(this);
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
		projection(0, 0) = scaleInv / aspectRatio;
		projection(1, 1) = scaleInv;
		projectionInverse(0, 0) = aspectRatio * scale;
		projectionInverse(1, 1) = scale;
	}
	else
	{
		projection(0, 0) = scaleInv;
		projection(1, 1) = scaleInv * aspectRatio;
		projectionInverse(0, 0) = scale;
		projectionInverse(1, 1) = scale / aspectRatio;
	}
	if(perspective)
	{
		float nf2 = 2 * near * far;
		float nmf = near - far;
		float npf = near + far;
		projection(2, 2) = npf / nmf;
		projection(2, 3) = nf2 / nmf;
		projection(3, 2) = -1;
		projection(3, 3) = 0;
		projectionInverse(2, 2) = 0;
		projectionInverse(2, 3) = -1;
		projectionInverse(3, 2) = nmf / nf2;
		projectionInverse(3, 3) = npf / nf2;
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
	Vector3f position = frame.getPosition();
	Matrix33f rot = frame.getOrientation().getMatrix();
	view(0, 0) = rot(0, 0);
	view(1, 0) = rot(0, 1);
	view(2, 0) = rot(0, 2);
	view(0, 1) = rot(2, 0);
	view(1, 1) = rot(2, 1);
	view(2, 1) = rot(2, 2);
	view(0, 2) = -rot(1, 0);
	view(1, 2) = -rot(1, 1);
	view(2, 2) = -rot(1, 2);
	view(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(2, 0) + position[2] * rot(1, 0));
	view(1, 3) = (-position[0] * rot(0, 1) - position[1] * rot(2, 1) + position[2] * rot(1, 1));
	view(2, 3) = (-position[0] * rot(0, 2) - position[1] * rot(2, 2) + position[2] * rot(1, 2));
	viewInverse(0, 0) = rot(0, 0);
	viewInverse(1, 0) = rot(1, 0);
	viewInverse(2, 0) = rot(2, 0);
	viewInverse(0, 1) = rot(0, 2);
	viewInverse(1, 1) = rot(1, 2);
	viewInverse(2, 1) = rot(2, 2);
	viewInverse(0, 2) = -rot(0, 1);
	viewInverse(1, 2) = -rot(1, 1);
	viewInverse(2, 2) = -rot(2, 1);
	viewInverse(0, 3) = position[0];
	viewInverse(1, 3) = position[1];
	viewInverse(2, 3) = position[2];
	viewNeedsUpdate = false;
}

