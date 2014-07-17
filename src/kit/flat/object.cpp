#include "object.h"

namespace kit
{
	namespace flat
	{
		Object::Object(Ptr<scene::Scene> scene)
		{
			if(!scene.isValid())
			{
				throw nullptr_exception();
			}
			_scene = scene;
			_object = scene->addObject();
			_model.setNew();
			_model->setVertexFormat(false, false, false, 1);
			std::vector<unsigned int> indices (6);
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 2;
			indices[4] = 3;
			indices[5] = 0;
			_model->setIndices(&indices[0], indices.size());
			_object->setModel(_model);
		}

		Object::~Object()
		{
			if(_scene.isValid())
			{
				_scene->removeObject(_object);
			}
			_model.setNull();
		}

		void Object::setPosition(Vector2f position)
		{
			if(_object.isValid())
			{
				Entity::setPosition(position);
				_object->setPosition(getPosition().extend<3>((float)getZ()));
			}
		}

		void Object::setOrientation(float orientation)
		{
			if(_object.isValid())
			{
				Entity::setOrientation(orientation);
				_object->setOrientation(Quaternionf(std::acos(2.0f * getOrientation()), 0, 0, std::asin(2.0f * getOrientation())));
			}
		}

		void Object::setZ(int z)
		{
			if(_object.isValid())
			{
				Entity::setZ(z);
				_object->setPosition(getPosition().extend<3>((float)getZ()));
			}
		}

		void Object::setTexture(std::string const & filename)
		{
			_model->clearTextures();
			_model->addTexture(filename, "diffuse", 0);
			updateVertices();
		}

		void Object::setTextureCoords(Recti coords)
		{
			_textureCoords = coords;
			updateVertices();
		}

		void Object::updateVertices()
		{
			Ptr<Texture> texture = _model->getTexture(0);
			if(texture.isValid())
			{
				Vector2i textureSize = _model->getTexture(0)->getSize();
				std::vector<float> vertices (20);
				vertices[0] = 0; vertices[1] = 0; vertices[2] = 0;
				vertices[3] = (float)_textureCoords.min[0] / textureSize[0]; vertices[4] = (float)_textureCoords.max[1] / textureSize[1];
				vertices[5] = (float)_textureCoords.getSize()[0]; vertices[6] = 0; vertices[7] = 0;
				vertices[8] = (float)_textureCoords.max[0] / textureSize[0]; vertices[9] = (float)_textureCoords.max[1] / textureSize[1];
				vertices[10] = (float)_textureCoords.getSize()[0]; vertices[11] = (float)_textureCoords.getSize()[1]; vertices[12] = 0;
				vertices[13] = (float)_textureCoords.max[0] / textureSize[0]; vertices[14] = (float)_textureCoords.min[1] / textureSize[1];
				vertices[15] = 0; vertices[16] = (float)_textureCoords.getSize()[1]; vertices[17] = 0;
				vertices[18] = (float)_textureCoords.min[0] / textureSize[0]; vertices[19] = (float)_textureCoords.min[1] / textureSize[1];
				_model->setVertices(&vertices[0], sizeof(float) * vertices.size());
			}
		}
	}
}

