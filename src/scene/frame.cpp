#include "frame.h"

namespace kit
{
	namespace internal
	{
		namespace scene
		{
			Frame::Frame ()
			{
				matrixInverse = matrix = Matrix44f::identity();
				matricesNeedUpdate = false;
			}

			Vector3f const & Frame::getPosition () const
			{
				return position;
			}

			void Frame::setPosition (Vector3f newPosition)
			{
				position = newPosition;
				matricesNeedUpdate = true;
			}

			Quaternionf const & Frame::getOrientation () const
			{
				return orientation;
			}

			void Frame::setOrientation (Quaternionf newOrientation)
			{
				orientation = newOrientation;
				matricesNeedUpdate = true;
			}

			Matrix44f const & Frame::getMatrix () const
			{
				if(matricesNeedUpdate)
				{
					const_cast<Frame *>(this)->updateMatrices();
				}
				return matrix;
			}

			Matrix44f  const & Frame::getMatrixInverse () const
			{
				if(matricesNeedUpdate)
				{
					const_cast<Frame *>(this)->updateMatrices();
				}
				return matrixInverse;
			}

			void Frame::updateMatrices ()
			{
				Matrix33f rot = orientation.getMatrix();
				matrix(0, 0) = rot(0, 0);
				matrix(1, 0) = rot(1, 0);
				matrix(2, 0) = rot(2, 0);
				matrix(0, 1) = rot(0, 1);
				matrix(1, 1) = rot(1, 1);
				matrix(2, 1) = rot(2, 1);
				matrix(0, 2) = rot(0, 2);
				matrix(1, 2) = rot(1, 2);
				matrix(2, 2) = rot(2, 2);
				matrix(0, 3) = position[0];
				matrix(1, 3) = position[1];
				matrix(2, 3) = position[2];
				matrixInverse(0, 0) = rot(0, 0);
				matrixInverse(1, 0) = rot(0, 1);
				matrixInverse(2, 0) = rot(0, 2);
				matrixInverse(0, 1) = rot(1, 0);
				matrixInverse(1, 1) = rot(1, 1);
				matrixInverse(2, 1) = rot(1, 2);
				matrixInverse(0, 2) = rot(2, 0);
				matrixInverse(1, 2) = rot(2, 1);
				matrixInverse(2, 2) = rot(2, 2);
				matrixInverse(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0));
				matrixInverse(1, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1));
				matrixInverse(2, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2));
				matricesNeedUpdate = false;
			}
		}
	}
}

