#pragma once

#include "entity.h"
#include "model.h"

namespace kit
{
	namespace scene
	{
		class Object : public Entity
		{
		public:
			float getScale() const;

			void setScale(float scale);

			Ptr<Model> getModel() const;

			void setModel(Ptr<Model> model);

			void setModel(std::string const & filename);

		private:
			Ptr<Model> model;
		};
	}
}

