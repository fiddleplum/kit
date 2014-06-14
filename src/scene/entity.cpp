#include "entity_p.h"

namespace kit
{
	namespace scene
	{
		EntityP::EntityP ()
		{
			worldToLocalMatrix = localToWorldMatrix = Matrix44f::identity();
			matricesNeedUpdate = false;
		}

		Vector3f const & EntityP::getPosition () const
		{
			return position;
		}

		void EntityP::setPosition (Vector3f newPosition)
		{
			position = newPosition;
			matricesNeedUpdate = true;
		}

		Quaternionf const & EntityP::getOrientation () const
		{
			return orientation;
		}

		void EntityP::setOrientation (Quaternionf newOrientation)
		{
			orientation = newOrientation;
			matricesNeedUpdate = true;
		}

		Matrix44f const & EntityP::getLocalToWorldMatrix () const
		{
			if(matricesNeedUpdate)
			{
				const_cast<EntityP *>(this)->updateMatrices();
			}
			return localToWorldMatrix;
		}

		Matrix44f  const & EntityP::getWorldToLocalMatrix () const
		{
			if(matricesNeedUpdate)
			{
				const_cast<EntityP *>(this)->updateMatrices();
			}
			return worldToLocalMatrix;
		}

		void EntityP::updateMatrices ()
		{
			Matrix33f rot = orientation.getMatrix();
			localToWorldMatrix(0, 0) = rot(0, 0);
			localToWorldMatrix(1, 0) = rot(1, 0);
			localToWorldMatrix(2, 0) = rot(2, 0);
			localToWorldMatrix(0, 1) = rot(0, 1);
			localToWorldMatrix(1, 1) = rot(1, 1);
			localToWorldMatrix(2, 1) = rot(2, 1);
			localToWorldMatrix(0, 2) = rot(0, 2);
			localToWorldMatrix(1, 2) = rot(1, 2);
			localToWorldMatrix(2, 2) = rot(2, 2);
			localToWorldMatrix(0, 3) = position[0];
			localToWorldMatrix(1, 3) = position[1];
			localToWorldMatrix(2, 3) = position[2];
			worldToLocalMatrix(0, 0) = rot(0, 0);
			worldToLocalMatrix(1, 0) = rot(0, 1);
			worldToLocalMatrix(2, 0) = rot(0, 2);
			worldToLocalMatrix(0, 1) = rot(1, 0);
			worldToLocalMatrix(1, 1) = rot(1, 1);
			worldToLocalMatrix(2, 1) = rot(1, 2);
			worldToLocalMatrix(0, 2) = rot(2, 0);
			worldToLocalMatrix(1, 2) = rot(2, 1);
			worldToLocalMatrix(2, 2) = rot(2, 2);
			worldToLocalMatrix(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0));
			worldToLocalMatrix(1, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1));
			worldToLocalMatrix(2, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2));
			matricesNeedUpdate = false;
		}
	}
}

