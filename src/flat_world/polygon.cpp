#include "polygon.h"
#include "../scene_2d.h"
#include "../texture.h"

namespace FlatWorld
{
	Polygon::Polygon(std::shared_ptr<Scene> newScene)
	{
		scene = newScene;

		boundingRect = Rectf::zero();

		color.set(1, 1, 1);

		model = std::make_shared<Model>();
		model->setNumIndicesPerPrimitive(3);
		model->setColor(Vector3f::zero(), color.extend<4>(1));
		model->setVertexFormat(false, false, false, 1);
	}

	Polygon::~Polygon()
	{
	}

	void Polygon::setPosition(Vector2f position)
	{
		Entity::setPosition(position);
		updateTransformedBoundingRect();
	}

	void Polygon::setOrientation(float orientation)
	{
		Entity::setOrientation(orientation);
		updateTransformedBoundingRect();
	}

	void Polygon::setScale(Vector2f newScale)
	{
		Entity::setScale(newScale);
		model->setScale(newScale.extend<3>(1));
		updateTransformedBoundingRect();
	}

	void Polygon::setZ(int newZ)
	{
		Entity::setZ(newZ);
		model->setPosition(getPosition().extend<3>((float)newZ));
		updateTransformedBoundingRect();
	}

	void Polygon::setVertices(std::vector<Vertex> newVertices)
	{
		vertices = newVertices;
		if(vertices.size() > 0)
		{
			boundingRect.min = boundingRect.max = vertices[0].p;
		}
		else
		{
			boundingRect = Rectf::zero();
		}
		for(Vertex const & v : vertices)
		{
			boundingRect.extendTo(v.p);
		}
		updateTransformedBoundingRect();
		updateModel();
	}

	void Polygon::setRectSprite(Vector2f size, Vector2i textureOffset, std::string textureFilename)
	{
		model->addTexture(textureFilename, "diffuse", 0);
		std::shared_ptr<Texture> texture = model->getTexture(0);
		Vector2f uvStart = textureOffset.scaleInv(texture->getSize());
		Vector2f uvSize = size.scaleInv(texture->getSize());
		vertices.push_back(Vertex(Vector2f(0, 0), Vector2f(uvStart[0], uvStart[1])));
		vertices.push_back(Vertex(Vector2f(size[0], 0), Vector2f(uvStart[0] + uvSize[0], uvStart[1])));
		vertices.push_back(Vertex(Vector2f(size[0], size[1]), Vector2f(uvStart[0] + uvSize[0], uvStart[1] + uvSize[1])));
		vertices.push_back(Vertex(Vector2f(0, size[1]), Vector2f(uvStart[0], uvStart[1] + uvSize[1])));
		boundingRect.min = Vector2f(0, 0);
		boundingRect.max = size;
		updateTransformedBoundingRect();
		updateModel();
	}

	void Polygon::setColor(Vector3f newColor)
	{
		color = newColor;
		model->setColor(Vector3f::zero(), color.extend<4>(1));
	}

	Rectf Polygon::getBoundingRect() const
	{
		return worldBoundingRect;
	}

	// This uses the edge intersections to find an intersection. It does not make the ray direction length set to the overlap.
	bool Polygon::intersects2(Polygon const & other, Ray2f & intersectionTowardOther) const
	{
		Polygon const * polygon [2] = {this, &other};
		std::vector<Vector2f> vs [2];
		for(unsigned int p = 0; p < 2; p++)
		{
			for(unsigned int i = 0; i < polygon[p]->vertices.size(); i++)
			{
				vs[p].push_back(polygon[p]->getWorldPositionOfVertex(i));
			}
		}
		Vector2f point0 [2];
		Vector2f point1 [2];
		for(unsigned int p = 0; p < 2; p++)
		{
			for(unsigned int i = 0; i < vs[p].size(); i++)
			{
				point0[p] = vs[p][i];
				point1[p] = vs[p][(i + 1) % vs[p].size()] - point0[p];
				for(unsigned int j = 0; j < vs[1 - p].size(); j++)
				{
					point0[1 - p] = vs[1 - p][j];
					point1[1 - p] = vs[1 - p][(j + 1) % vs[1 - p].size()] - point0[1 - p];

					// find intersection between edges
					float d = point1[p].cross2d(point1[1 - p]);
					if(d == 0)
					{
						continue; // parallel or collinear
					}
					Vector2f x = point0[1 - p] - point0[p];
					float t = x.cross2d(point1[1 - p]) / d;
					float u = x.cross2d(point1[p]) / d;
					if(0 < t && t < 1 && 0 < u && u < 1) // they intersect
					{
						if(std::min(t, 1.0f - t) > std::min(u, 1.0f - u)) // t is the edge, u is the point
						{
							intersectionTowardOther.direction = -point1[p].perp2d().unit();
							intersectionTowardOther.start = point0[1 - p];
							if(u > 0.5f)
							{
								intersectionTowardOther.start += point1[1 - p];
							}
						}
						else
						{
							intersectionTowardOther.direction = point1[1 - p].perp2d().unit();
							intersectionTowardOther.start = point0[p];
							if(t > 0.5f)
							{
								intersectionTowardOther.start += point1[p];
							}
						}
						return true;
					}
				}
			}
		}
		return false;
	}

	void Polygon::project(Ray2f ray, float & min, unsigned int & minI, float & max, unsigned int & maxI) const
	{
		min = max = 0;
		minI = maxI = 0;
		for(unsigned int i = 0; i < vertices.size(); i++)
		{
			float x = (getWorldPositionOfVertex(i) - ray.start).dot(ray.direction);
			if(x < min || i == 0)
			{
				min = x;
				minI = i;
			}
			if(x > max || i == 0)
			{
				max = x;
				maxI = i;
			}
		}
	}

	Vector2f Polygon::getWorldPositionOfVertex(unsigned int i) const
	{
		Vector2f v = vertices[i].p;
		v = v.scale(getScale());
		v.set(v[0] * cos(getOrientation()) - v[1] * sin(getOrientation()), v[1] * cos(getOrientation()) + v[0] * sin(getOrientation()));
		v += getPosition();
		return v;
	}

	void Polygon::updateTransformedBoundingRect()
	{
		Vector2f verts[4];
		verts[0].set(boundingRect.min[0], boundingRect.min[1]);
		verts[1].set(boundingRect.max[0], boundingRect.min[1]);
		verts[2].set(boundingRect.min[0], boundingRect.max[1]);
		verts[3].set(boundingRect.max[0], boundingRect.max[1]);
		for(unsigned int i = 0; i < 4; i++)
		{
			verts[i].scale(getScale());
			verts[i].set(
				verts[i][0] * cos(getOrientation()) - verts[i][1] * sin(getOrientation()),
				verts[i][1] * cos(getOrientation()) + verts[i][0] * sin(getOrientation()));
			verts[i] += getPosition();
			if(i == 0)
			{
				worldBoundingRect.min = worldBoundingRect.max = verts[i];
			}
			else
			{
				worldBoundingRect.extendTo(verts[i]);
			}
		}
	}

	void Polygon::updateModel()
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
			if(i >= 2)
			{
				is.push_back(0);
				is.push_back((vs.size() / numFloatPerVertex) - 2);
				is.push_back((vs.size() / numFloatPerVertex) - 1);
			}
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
}

