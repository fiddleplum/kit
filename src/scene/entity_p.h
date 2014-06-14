#pragma once

#include <kit/matrix.h>
#include <kit/scene/entity.h>

namespace kit
{
	namespace scene
	{
		class EntityP : virtual public Entity
		{
		public:
			EntityP ();

			Vector3f const & getPosition () const override;

			void setPosition (Vector3f) override;

			Quaternionf const & getOrientation () const override;

			void setOrientation (Quaternionf) override;

			Matrix44f const & getLocalToWorldMatrix () const;

			Matrix44f const & getWorldToLocalMatrix () const;

		private:
			void updateMatrices ();

			Vector3f position;
			Quaternionf orientation;
			bool matricesNeedUpdate;
			Matrix44f localToWorldMatrix;
			Matrix44f worldToLocalMatrix;
		};
	}
}

