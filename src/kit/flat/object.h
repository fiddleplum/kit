#pragma once

#include "entity.h"
#include "../scene.h"

namespace kit
{
	class Model;

	namespace flat
	{
		class Object : public Entity
		{
		public:
			Object(Ptr<Scene> scene);

			~Object();

			Ptr<kit::Object> getSceneObject() const { return _object; }

			void setPosition(Vector2f position) override;

			void setOrientation(float orientation) override;

			void setZ(int z) override;

			void setTexture(std::string const & filename);

			void setTextureCoords(Recti coords);

		private:
			void updateVertices();

			Ptr<Scene> _scene;
			Ptr<kit::Object> _object;
			OwnPtr<Model> _model;
			Recti _textureCoords;
		};
	}
}

