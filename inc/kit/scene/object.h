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
			virtual float getScale() const = 0;

			virtual void setScale(float scale) = 0;

			virtual Ptr<Model> getModel() const = 0;

			virtual void setModel(Ptr<Model> model) = 0;

			virtual void setModel(std::string const & filename) = 0;
		};
	}
}

