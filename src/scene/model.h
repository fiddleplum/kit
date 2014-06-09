#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <kit/noncopyable.h>
#include <vector>
#include <istream>
#include <string>
#include <set>

namespace kit
{
	class VertexBufferObject;
	class Texture;
	class Shader;

	namespace scene
	{
		class CameraInternal;
		class EntityInternal;

		// The model serialized format is at the bottom.

		class Model : public Noncopyable
		{
		public:
			Model ();

			void load (std::string const & filename);

			void setVertexFormat (bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

			void setVertices (void const * vertices, unsigned int numBytes);

			void setNumIndicesPerPrimitive (unsigned int num);

			void setIndices (unsigned int const * indices, unsigned int numIndices);

			Ptr<Texture> getTexture (unsigned int textureIndex) const;

			void addTexture (std::string const & filename, std::string const & type, unsigned int uvIndex);

			void clearTextures ();

			void setColor (Vector3f const & emitColor, Vector4f const & diffuseColor);

			void setSpecular (unsigned int level, float strength);
		
			void render (Ptr<CameraInternal> camera, Ptr<EntityInternal> entity, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

			bool setNeedsResorting () const;

			void resortingDone ();

			bool operator < (Model const & model) const;

			static const unsigned int maxLights = 4;

		private:
			class TextureInfo
			{
			public:
				Ptr<Texture> texture;
				std::string type;
				int samplerLocation;
				int uvIndex;
			};

			void updateShader();

			Vector3f emitColor;
			int emitColorLocation;
			Vector4f diffuseColor;
			int diffuseColorLocation;
			unsigned int specularLevel;
			int specularLevelLocation;
			float specularStrength;
			int specularStrengthLocation;

			int lightPositionsLocation;
			int lightColorsLocation;

			std::vector<TextureInfo> textureInfos;

			bool vertexHasNormal;
			bool vertexHasTangent;
			bool vertexHasColor;
			unsigned int numVertexUVs;
			unsigned int numBytesPerVertex;
			OwnPtr<VertexBufferObject> vertexBufferObject;

			Ptr<Shader> shader;
			bool needsResorting;

			int projectionLocation;
			int worldViewLocation;
		};
	}
}

