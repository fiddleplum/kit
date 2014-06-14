#pragma once

#include <kit/ptr.h>
#include <kit/noncopyable.h>
#include <kit/vector.h>
#include <string>

namespace kit
{
	class Texture;

	namespace scene
	{
		class Model : Noncopyable
		{
		public:
			virtual void setVertexFormat (bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs) = 0;

			virtual void setVertices (void const * vertices, unsigned int numBytes) = 0;

			virtual void setNumIndicesPerPrimitive (unsigned int num) = 0;

			virtual void setIndices (unsigned int const * indices, unsigned int numIndices) = 0;

			virtual Ptr<Texture> getTexture (unsigned int textureIndex) const = 0;

			virtual void addTexture (Ptr<Texture> texture, std::string const & type, unsigned int uvIndex) = 0;

			virtual void addTexture (std::string const & filename, std::string const & type, unsigned int uvIndex) = 0;

			virtual void clearTextures () = 0;

			virtual void setColor (Vector3f const & emitColor, Vector4f const & diffuseColor) = 0;

			virtual void setSpecular (unsigned int level, float strength) = 0;

			virtual void setScale (float scale) = 0;
		};
	}
}

