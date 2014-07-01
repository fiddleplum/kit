#include "object.h"
#include "../resources.h"

namespace kit
{
	namespace scene
	{
		float Object::getScale () const
		{
			model->getScale();
		}

		void Object::setScale (float scale)
		{
			model->setScale(scale);
		}

		Ptr<Model> Object::getModel () const
		{
			return model;
		}

		void Object::setModel (Ptr<Model> model) 
		{
			this->model = model;
		}

		void Object::setModel (std::string const & filename)
		{
			this->model = resources::getModelFromFile(filename);
		}
	}
}

