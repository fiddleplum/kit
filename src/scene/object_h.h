#pragma once

#include <kit/scene/object.h>
#include "model_p.h"
#include "entity_p.h"

namespace kit
{
	namespace scene
	{
		class ObjectP : public Object, public EntityP
		{
		public:
			float getScale () const override;

			void setScale (float scale) override;

			UsePtr<Model> getModel () const override;

			void setModel (UsePtr<Model> model) override;

			void setModel (std::string const & filename) override;

		private:
			UsePtr<ModelP> model;
		};
	}
}

