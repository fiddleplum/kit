#pragma once

#include <kit/scene/entity.h>
#include <kit/rect.h>
#include <string>

namespace kit
{
	namespace scene
	{
		class Object : virtual public Entity
		{
		public:
			virtual Vector3f getScale () const = 0;

			virtual void setScale (Vector3f scale) = 0;

			virtual Ptr<Model> getModel () const = 0;

			virtual void setModelAsNew () = 0;

			virtual void setModel (std::string const & filename) = 0;

			virtual void setModelAsSprite (std::string const & texture, Recti const & textureCoords) = 0;
		};
	}
}

