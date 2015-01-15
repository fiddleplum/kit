#pragma once

#include "coord.h"
#include "ptr.h"
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
		Coord2i pos;
		Coord2i uv;
	};

	// Constructor.
	GuiModel();

	// Returns the position.
	Coord2i getPosition() const;

	// Sets the position.
	void setPosition(Coord2i position);

	// Returns the texture.
	Ptr<Texture> getTexture() const;

	// Sets the texture.
	void setTexture(Ptr<Texture> texture);

	// Sets the vertices.
	void setVertices(std::vector<Vertex> const & vertices);

	// Sets the indices.
	void setIndices(std::vector<unsigned int> const & indices);

	// Renders the model.
	void render(Coord2i windowSize);

private:
	void updateShader();

	Coord2i position;
	Ptr<Texture> texture;
	Ptr<Shader> shader;
	OwnPtr<VertexBufferObject> vbo;
	int windowSizeLocation;
	int positionLocation;
	int textureSizeLocation;
	int samplerLocation;
};

