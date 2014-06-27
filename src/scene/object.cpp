#include "object_p.h"
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

		Ptr<Model> ObjectP::getModel () const
		{
			return model;
		}

		void ObjectP::setModel (Ptr<Model> model) 
		{
			this->model = model.as<ModelP>();
		}

		void ObjectP::setModel (std::string const & filename)
		{
			this->model = resources::getModelFromFile(filename).as<ModelP>();
		}
	}
}

