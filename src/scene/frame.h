#pragma once

#include <kit/matrix.h>
#include <kit/vector.h>
#include <kit/quaternion.h>

namespace kit
{
	namespace internal
	{
		namespace scene
		{
			class Frame
			{
			public:
				Frame ();

				Vector3f const & getPosition () const;

				void setPosition (Vector3f);

				Quaternionf const & getOrientation () const;

				void setOrientation (Quaternionf);

				Matrix44f const & getMatrix () const;

				Matrix44f const & getMatrixInverse () const;

			private:
				void updateMatrices ();

				Vector3f position;
				Quaternionf orientation;
				bool matricesNeedUpdate;
				Matrix44f matrix;
				Matrix44f matrixInverse;
			};
		}
	}
}

