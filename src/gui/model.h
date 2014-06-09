#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <memory>
#include <string>
#include <vector>

namespace kit
{
	class Texture;
	class Shader;
	class VertexBufferObject;
	class Window;

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

			Model();
			Vector2i getPosition() const;
			void setPosition(Vector2i position);
			void setTexture(std::string const & filename);
			void setVertices(std::vector<Vertex> const & vertices);
			void setIndices(std::vector<unsigned int> const & indices);
			void render(Vector2i windowSize);

		private:
			Vector2i position;
			Ptr<Texture> texture;
			Ptr<Shader> shader;
			OwnPtr<VertexBufferObject> vbo;
			int windowSizeLocation;
			int positionLocation;
			int textureSizeLocation;
			int samplerLocation;
		};
	}
}

