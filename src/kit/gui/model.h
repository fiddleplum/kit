#pragma once

#include "../vector.h"
#include "../ptr.h"
#include "../texture.h"
#include <string>
#include <vector>

namespace kit
{
	class Shader;
	class VertexBufferObject;

	namespace gui
	{
		class Model
		{
		public:
			class Vertex
			{
			public:
				Vector2f pos;
				Vector2f uv;
			};

			Model ();

			Vector2i getPosition () const;
			void setPosition (Vector2i position);

			Ptr<Texture> getTexture () const;
			void setTexture (Ptr<Texture> texture);

			void setVertices (std::vector<Vertex> const & vertices);
			void setIndices (std::vector<unsigned int> const & indices);
			void render (Vector2i windowSize);

		private:
			Vector2i _position;
			Ptr<Texture> _texture;
			Ptr<Shader> _shader;
			OwnPtr<VertexBufferObject> _vbo;
			int _windowSizeLocation;
			int _positionLocation;
			int _textureSizeLocation;
			int _samplerLocation;
		};
	}
}

