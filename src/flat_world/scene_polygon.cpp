#include "scene_polygon.h"
#include "../texture.h"

ScenePolygon::ScenePolygon(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void ScenePolygon::setPosition(Vector2f newPosition)
{
	Entity::setPosition(newPosition);
	model->setPosition(newPosition.extend<3>((float)getZ()));
	updateTransformedBoundingRect();
}

void ScenePolygon::setOrientation(float orientation)
{
	model->setOrientation(Quaternionf(orientation, Vector3f::axis(2), true));
}

void ScenePolygon::setScale(Vector2f scale)
{
	model->setScale(scale.extend<3>(1));
}

void ScenePolygon::setZ(int z)
{
	Vector3f position = model->getPosition();
	position[2] = (float)z;
	model->setPosition(position);
}

void ScenePolygon::setVertices(std::vector<Vertex> const & vertices)
{
	unsigned int numFloatPerVertex = 5;
	std::vector<float> vs;
	std::vector<unsigned int> is;
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		auto const & v = vertices[i];
		vs.push_back(v.p[0]);
		vs.push_back(v.p[1]);
		vs.push_back(0.0f);
		vs.push_back(v.uv[0]);
		vs.push_back(v.uv[1]);
		is.push_back(i);
	}
	if(vs.size() > 0)
	{
		model->setVertices(&vs[0], sizeof(float) * vs.size());
	}
	if(is.size() > 0)
	{
		model->setIndices(&is[0], is.size());
	}
}

void ScenePolygon::setAsSprite(Vector2i size, Vector2i textureOffset, std::string textureFilename)
{
	setTexture(textureFilename);
	std::shared_ptr<Texture> texture = getTexture();

	Vector2f uvStart = textureOffset.scaleInv(texture->getSize());
	Vector2f uvSize = Vector2f(size).scaleInv(texture->getSize());
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(Vector2f(0, 0), Vector2f(uvStart[0], uvStart[1])));
	vertices.push_back(Vertex(Vector2f((float)size[0], 0), Vector2f(uvStart[0] + uvSize[0], uvStart[1])));
	vertices.push_back(Vertex(Vector2f((float)size[0], (float)size[1]), Vector2f(uvStart[0] + uvSize[0], uvStart[1] + uvSize[1])));
	vertices.push_back(Vertex(Vector2f(0, (float)size[1]), Vector2f(uvStart[0], uvStart[1] + uvSize[1])));
	setVertices(vertices);
}

std::shared_ptr<Texture> ScenePolygon::getTexture() const
{
	return model->getTexture(0);
}

void ScenePolygon::setTexture(std::string const & filename)
{
	model->clearTextures();
	model->addTexture(filename, "diffuse", 0);
}

