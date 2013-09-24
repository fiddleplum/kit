#pragma once

#include "../Vector.h"
#include <memory>
#include <string>
#include <vector>

class Texture;
class Shader;
class VertexBufferObject;

namespace Gui
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
		void render();

	private:
		Vector2i position;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexBufferObject> vbo;
		int windowSizeLocation;
		int positionLocation;
		int textureSizeLocation;
		int samplerLocation;
	};
}

