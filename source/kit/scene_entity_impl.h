#pragma once

#include <kit/scene_entity.h>

class SceneEntityImpl : public virtual SceneEntity
{
public:
	SceneEntityImpl();

	Vector3f const & getPosition() const override;

	void setPosition(Vector3f position) override;

	Quaternionf const & getOrientation() const override;

	void setOrientation(Quaternionf orientation) override;

	Matrix44f const & getLocalToWorldTransform() const override;

	Matrix44f const & getWorldToLocalTransform() const override;

private:
	void updateTransforms();

	Vector3f position;
	Quaternionf orientation;
	bool transformsNeedUpdate;
	Matrix44f localToWorldTransform;
	Matrix44f worldToLocalTransform;
};

