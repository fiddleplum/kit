#include "frame_2d.h"

Frame2D::Frame2D()
{
	position.set(0.0f, 0.0f);
	orientation = 0.0f;
	transformsDirty = true;
	transform = transformInv = Matrix33f::identity();
}

void Frame2D::setPosition(Vector2f newPosition)
{
	position = newPosition;
	transformsDirty = true;
}

void Frame2D::setOrientation(float newOrientation)
{
	orientation = newOrientation;
	transformsDirty = true;
}

Matrix33f const & Frame2D::getTransform() const
{
	if(transformsDirty)
	{
		const_cast<Frame2D *>(this)->updateTransforms();
	}
	return transform;
}

Matrix33f const & Frame2D::getTransformInv() const
{
	if(transformsDirty)
	{
		const_cast<Frame2D *>(this)->updateTransforms();
	}
	return transformInv;
}

void Frame2D::updateTransforms()
{
	Vector2f basis [2];
	basis[0].set(cos(orientation), sin(orientation));
	basis[1] = basis[0].perp2d();

	transform(0, 0) = basis[0][0];
	transform(1, 0) = basis[0][1];
	transform(0, 1) = basis[1][0];
	transform(1, 1) = basis[1][1];
	transform(0, 2) = position[0];
	transform(1, 2) = position[1];
	transformInv(0, 0) = basis[0][0];
	transformInv(1, 0) = basis[1][0];
	transformInv(0, 1) = basis[0][1];
	transformInv(1, 1) = basis[1][1];
	transformInv(0, 2) = -basis[0].dot(position);
	transformInv(1, 2) = -basis[1].dot(position);
	transformsDirty = false;
}

