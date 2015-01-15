#pragma once

#include "entity.h"
#include "../scene/scene.h"

namespace kit
{
	namespace scene
	{
		class Model;
	}

	namespace flat
	{
		class Object : public Entity
		{
		public:
			class Vertex
			{
			public:
				Coord2f position;
				float _reserved = 0;
				Coord2f textureCoord;
			};

			Object(Ptr<scene::Scene> scene);

			~Object();

			Ptr<scene::Object> getSceneObject() const { return _object; }

			void setPosition(Coord2f position) override;

			void setOrientation(float orientation) override;

			void setScale(float scale);

			void setZ(int z) override;

			Ptr<Texture> getTexture() const { return _object->getModel()->getTexture(0); }

			void setTexture(std::string const & filename);

			void setAsSprite(Coord2f bottomLeftOffsetFromPosition, Recti textureCoords);

			void setVertices(std::vector<Vertex> const & vertices);

			void setIndices(std::vector<unsigned int> const & indices);

		private:
			Ptr<scene::Scene> _scene;
			Ptr<scene::Object> _object;
			OwnPtr<scene::Model> _model;
		};
	}
}

