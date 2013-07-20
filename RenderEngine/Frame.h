#pragma once

#include "../Vector.h"
#include "../Quaternion.h"
#include "../Matrix.h"

class Frame
{
public:
	Frame();

	Vector3f const & getPosition() const;
	void setPosition(Vector3f);

	Quaternionf const & getOrientation() const;
	void setOrientation(Quaternionf);

	Vector3f const & getScale() const;
	void setScale(Vector3f);

	Matrix44f const & getMatrix() const;
	Matrix44f  const & getMatrixInverse() const;

private:
	void updateMatrices();

	Vector3f position;
	Quaternionf orientation;
	Vector3f scale;
	bool matricesNeedUpdate;
	Matrix44f matrix;
	Matrix44f matrixInverse;
};

