#include "Camera.h"
#include <exception>

Camera::Camera()
{
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
	return (getProjection() * getView()).transform(positionInWorld, 1).shink<2>();
}

Ray3f Camera::getRay(Vector2f ndcPosition)
{
	Ray3f ray;
	ray.start = frame.getPosition();
	if(perspective)
	{
		ndcPosition = 
	}
	else
	{
	}
	ray.direction = frame.getMatrixInverse() * (ndcPosition;
	unit(transform(world_matrix(), Vector<3, float>(
	(((float) position_in_window(0) /(window_size(0) / 2)) - 1.0f),
	focal_length,(((float) position_in_window(1) / -(window_size(1) / 2))
	+ 1.0f)), 0.0f));
	return ray;
}

void Camera::activate()
{
	glViewport(viewport.min[0], viewport.min[1], viewport.max[0] - viewport.min[0], viewport.max[1] - viewport.min[1]);
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
	if(scale == 0)
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
		float nf2 = 2.0f * near * far;
		float nmf = near - far;
		float npf = near + far;
		projection(2, 2) = npf / nmf;
		projection(2, 3) = nf2 / nmf;
		projection(3, 2) = -1.0f;
		projection(3, 3) = 0.0f;
		projectionInverse(2, 2) = 0;
		projectionInverse(2, 3) = -1;
		projectionInverse(3, 2) = nmf / nf2;
		projectionInverse(3, 3) = npf / nf2;
	}
	else
	{
		projection(2, 2) = 0;
		projection(3, 2) = 0.0f;
		projection(3, 3) = 1.0f;
		// TODO: projection and projectionInverse need to be finished
	}
	projectionNeedsUpdate = false;
}

void Camera::updateView()
{
	Vector3f position = frame.getPosition();
	Matrix<3, 3, T> rot = frame.getOrientation().getMatrix();
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
	vewInverse(0, 0) = rot(0, 0);
	vewInverse(1, 0) = rot(1, 0);
	vewInverse(2, 0) = rot(2, 0);
	vewInverse(0, 1) = rot(0, 2);
	vewInverse(1, 1) = rot(1, 2);
	vewInverse(2, 1) = rot(2, 2);
	vewInverse(0, 2) = -rot(0, 1);
	vewInverse(1, 2) = -rot(1, 1);
	vewInverse(2, 2) = -rot(2, 1);
	vewInverse(0, 3) = position[0];
	vewInverse(1, 3) = position[1];
	vewInverse(2, 3) = position[2];
	viewNeedsUpdate = false;
}

