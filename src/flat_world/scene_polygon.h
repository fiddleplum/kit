#pragma once

#include "../model.h"

class Scene;

class ScenePolygon
{
public:
	class Vertex
	{
	public:
		Vertex(Vector2f p, Vector2f uv) { this->p = p; this->uv = uv; }
		Vector2f p;
		Vector2f uv;
	};

	ScenePolygon(std::shared_ptr<Scene>);

	void setPosition(Vector2f);
	void setOrientation(float);
	void setScale(Vector2f);
	void setZ(int);

	void setVertices(std::vector<Vertex> const & );

	std::shared_ptr<Texture> getTexture() const;
	void setTexture(std::string const & filename);

	void setAsSprite(Vector2i size, Vector2i uvStart, std::string textureFilename);

private:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Model> model;
};

