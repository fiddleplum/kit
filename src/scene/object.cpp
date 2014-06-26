#include "object_h.h"
#include "../resources_p.h"

namespace kit
{
	namespace scene
	{
		float ObjectP::getScale () const
		{
			model->getScale();
		}

		void ObjectP::setScale (float scale)
		{
			model->setScale(scale);
		}

		UsePtr<Model> ObjectP::getModel () const
		{
			return model;
		}

		void ObjectP::setModel (UsePtr<Model> model) 
		{
			this->model = model.as<ModelP>();
		}

		void ObjectP::setModel (std::string const & filename)
		{
			this->model = resources::getModelFromFile(filename).as<ModelP>();
		}
	}
}

