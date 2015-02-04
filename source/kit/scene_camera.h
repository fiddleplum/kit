#pragma once

#include "scene_entity.h"
#include "ray.h"

class SceneCamera : public SceneEntity
{
public:
	SceneCamera();

	void setPosition(Coord3f position) override;

	void setOrientation(Quaternionf orientation) override;

	void setAspectRatio(float aspectRatio);

	void setNear(float near);

	void setFar(float far);

	void setPerspective(float fov);

	void setOrthogonal(float size);

	Coord2f getNdcPosition(Coord3f worldPosition) const;

	Ray3f getRay(Coord2f ndcPosition) const;

	Matrix44f const & getWorldToCameraTransform() const;

	Matrix44f const & getCameraToNdcTransform() const;

private:
	void updateWorldToCamera();
	void updateCameraToNdc();

	float aspectRatio;
	float near;
	float far;
	float fov;
	float size;
	bool perspective;
	bool cameraToNdcTransformNeedsUpdate;
	bool worldToCameraTransformNeedsUpdate;
	Matrix44f cameraToNdcTransform;
	Matrix44f ndcToCameraTransform;
	Matrix44f worldToCameraTransform;
	Matrix44f cameraToWorldTransform;
};

