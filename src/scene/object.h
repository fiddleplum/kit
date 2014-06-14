#pragma once

#include <kit/scene/object.h>
#include "model.h"
#include "frame.h"

namespace kit
{
	namespace scene
	{
		namespace internal
		{
			class Object : virtual public scene::Object
			{
			public:
				Vector3f const & getPosition () const override;

				void setPosition (Vector3f position) override;

				Quaternionf const & getOrientation () const override;

				void setOrientation (Quaternionf orientation) override;

				Vector3f getScale () const override;

				void setScale (Vector3f scale) override;

				void setModel (std::string const & filename) override;

				void setModelAsSprite (std::string const & texture, Recti const & textureCoords) override;

			private:
				Frame frame;
				OwnPtr<Model> model;
			};
		}
	}
}

