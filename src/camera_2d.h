#pragma once

#include "frame_2d.h"

class Camera2D
{
public:
	Camera2D();

	Vector2f getPosition() const { return frame.getPosition(); }
	void setPosition(Vector2f position) { frame.setPosition(position); }

	float getOrientation() const { return frame.getOrientation(); }
	void setOrientation(float orientation) { frame.setOrientation(orientation); }

	float getAspectRatio() const { return aspectRatio; }
	void setAspectRatio(float);

	float getMaxViewSize() const { return maxViewSize; }
	void setMaxViewSize(float);

	Vector2f getViewSize() const { return viewSize; }
	Matrix33f const & getViewTransform() const { return frame.getTransformInv(); }

	Vector2f getNdcPosition(Vector2f worldPosition) const;
	Vector2f getWorldPosition(Vector2f ndcPosition) const;

private:
	void updateViewSize();

	Frame2D frame;
	float aspectRatio;
	float maxViewSize;
	Vector2f viewSize;
};

