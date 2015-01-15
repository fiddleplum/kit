#pragma once

#include <kit/scene_entity.h>

class SceneEntityImpl : public virtual SceneEntity
{
public:
	SceneEntityImpl();

	Coord3f const & getPosition() const override;

	void setPosition(Coord3f position) override;

	Quaternionf const & getOrientation() const override;

	void setOrientation(Quaternionf orientation) override;

	Matrix44f const & getLocalToWorldTransform() const override;

	Matrix44f const & getWorldToLocalTransform() const override;

private:
	void updateTransforms();

	Coord3f position;
	Quaternionf orientation;
	bool transformsNeedUpdate;
	Matrix44f localToWorldTransform;
	Matrix44f worldToLocalTransform;
};

