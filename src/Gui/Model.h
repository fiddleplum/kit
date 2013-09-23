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
			Vector2i pos;
			Vector2i uv;
		};

		Model();
		void setTexture(std::string const & filename);
		void setVertices(std::vector<Vertex> const & vertices);
		void setNumFrames(int numFrames);
		void setFrame(int frame);
		void render(Vector2i windowSize);

	private:
		void initShader();

		std::shared_ptr<Texture> texture;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexBufferObject> vbo;
		int numFrames;
	};
}

