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
			Object(Ptr<scene::Scene> scene);

			~Object();

			Ptr<scene::Object> getSceneObject() const { return _object; }

			void setPosition(Vector2f position) override;

			void setOrientation(float orientation) override;

			void setZ(int z) override;

			void setTexture(std::string const & filename);

			void setTextureCoords(Recti coords);

		private:
			void updateVertices();

			Ptr<scene::Scene> _scene;
			Ptr<scene::Object> _object;
			OwnPtr<scene::Model> _model;
			Recti _textureCoords;
		};
	}
}

