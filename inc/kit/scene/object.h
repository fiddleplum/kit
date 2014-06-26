#pragma once

#include <kit/scene/entity.h>
#include <kit/rect.h>
#include <kit/ptr.h>
#include <string>

namespace kit
{
	namespace scene
	{
		class Model;

		class Object : virtual public Entity
		{
		public:
			virtual float getScale () const = 0;

			virtual void setScale (float scale) = 0;

			virtual UsePtr<Model> getModel () const = 0;

			virtual void setModel (UsePtr<Model> model) = 0;

			virtual void setModel (std::string const & filename) = 0;
		};
	}
}

