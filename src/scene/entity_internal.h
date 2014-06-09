#pragma once

#include <kit/matrix.h>
#include <kit/scene/entity.h>

namespace kit
{
	namespace scene
	{
		class EntityInternal : virtual public Entity
		{
		public:
			EntityInternal ();

			Vector3f const & getPosition () const override;

			virtual void setPosition (Vector3f) override;

			Quaternionf const & getOrientation () const override;

			virtual void setOrientation (Quaternionf) override;

			Vector3f const & getScale () const override;

			virtual void setScale (Vector3f) override;

			Matrix44f const & getMatrix () const;

			Matrix44f const & getMatrixInverse () const;

		private:
			void updateMatrices ();

			Vector3f position;
			Quaternionf orientation;
			Vector3f scale;
			bool matricesNeedUpdate;
			Matrix44f matrix;
			Matrix44f matrixInverse;
		};
	}
}

