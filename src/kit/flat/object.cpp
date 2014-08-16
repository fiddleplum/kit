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
			_model.create();
			_model->setVertexFormat(false, false, false, 1);
			_object->setModel(_model);
		}

		Object::~Object()
		{
			if(_scene.isValid())
			{
				_scene->removeObject(_object);
			}
			_model.destroy();
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
				_object->setOrientation(Quaternionf(std::cos(getOrientation() / 2.f), 0, 0, std::sin(getOrientation() / 2.f)));
			}
		}

		void Object::setScale(float scale)
		{
			if(_object.isValid())
			{
				_object->setScale(scale);
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
		}

		void Object::setAsSprite(Vector2f offsetFromPosition, Recti textureCoords)
		{
			std::vector<unsigned int> indices(6);
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 2;
			indices[4] = 3;
			indices[5] = 0;
			setIndices(indices);

			Ptr<Texture> texture = _model->getTexture(0);
			if(texture.isValid())
			{
				Vector2i textureSize = _model->getTexture(0)->getSize();
				std::vector<Vertex> vertices(4);
				vertices[0].position.set(offsetFromPosition[0], offsetFromPosition[1]);
				vertices[0].textureCoord.set((float)textureCoords.min[0] / textureSize[0], (float)textureCoords.max[1] / textureSize[1]);
				vertices[1].position.set(offsetFromPosition[0] + (float)textureCoords.getSize()[0], offsetFromPosition[1]);
				vertices[1].textureCoord.set((float)textureCoords.max[0] / textureSize[0], (float)textureCoords.max[1] / textureSize[1]);
				vertices[2].position.set(offsetFromPosition[0] + (float)textureCoords.getSize()[0], offsetFromPosition[1] + (float)textureCoords.getSize()[1]);
				vertices[2].textureCoord.set((float)textureCoords.max[0] / textureSize[0], (float)textureCoords.min[1] / textureSize[1]);
				vertices[3].position.set(offsetFromPosition[0], offsetFromPosition[1] + (float)textureCoords.getSize()[1]);
				vertices[3].textureCoord.set((float)textureCoords.min[0] / textureSize[0], (float)textureCoords.min[1] / textureSize[1]);
				setVertices(vertices);
			}
		}

		void Object::setVertices(std::vector<Vertex> const & vertices)
		{
			_model->setVertices(&vertices[0], sizeof(Vertex)* vertices.size());
		}

		void Object::setIndices(std::vector<unsigned int> const & indices)
		{
			_model->setIndices(&indices[0], indices.size());
		}
	}
}

