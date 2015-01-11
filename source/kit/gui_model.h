#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <string>
#include <vector>

class Texture;
class Shader;
class VertexBufferObject;

class GuiModel
{
public:
	class Vertex
	{
	public:
		Vector2i pos;
		Vector2i uv;
	};

	GuiModel();

	Vector2i getPosition() const;
	void setPosition(Vector2i position);

	Ptr<Texture> getTexture() const;
	void setTexture(Ptr<Texture> texture);

	void setVertices(std::vector<Vertex> const & vertices);
	void setIndices(std::vector<unsigned int> const & indices);
	void render(Vector2i windowSize);

private:
	void updateShader();

	Vector2i position;
	Ptr<Texture> texture;
	Ptr<Shader> shader;
	OwnPtr<VertexBufferObject> vbo;
	int windowSizeLocation;
	int positionLocation;
	int textureSizeLocation;
	int samplerLocation;
};

