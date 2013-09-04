#pragma once

#include "Entity.h"
#include "Ray.h"

class Scene;

class Camera : public Entity
{
public:
	Camera(Scene *);
	void setAspectRatio(float);
	void setNear(float);
	void setFar(float);
	void setPerspective(float);
	void setOrthogonal(float);
	virtual void setPosition(Vector3f) override;
	virtual void setOrientation(Quaternionf) override;
	virtual void setScale(Vector3f) override;
	Vector2f getNdcPosition(Vector3f worldPosition) const;
	Ray3f getRay(Vector2f ndcPosition) const;
	Matrix44f const & getProjection() const;
	Matrix44f const & getView() const;
	void render();

private:
	void updateProjection();
	void updateView();

	Scene * scene;
	float aspectRatio;
	float near;
	float far;
	float fov;
	float size;
	bool perspective;
	bool projectionNeedsUpdate;
	bool viewNeedsUpdate;
	Matrix44f projection;
	Matrix44f projectionInverse;
	Matrix44f view;
	Matrix44f viewInverse;
};

