#pragma once

#include <kit/scene/model.h>
#include <vector>
#include <istream>
#include <set>

namespace kit
{
	class VertexBufferObject;
	class TextureP;
	class Shader;

	namespace scene
	{
		class EntityP;
		class CameraP;

		class ModelP : virtual public Model
		{
		public:
			ModelP ();

			ModelP (std::string const & filename);

			void setVertexFormat (bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs) override;

			void setVertices (void const * vertices, unsigned int numBytes) override;

			void setNumIndicesPerPrimitive (unsigned int num) override;

			void setIndices (unsigned int const * indices, unsigned int numIndices) override;

			Ptr<Texture> getTexture (unsigned int textureIndex) const override;

			void addTexture (Ptr<Texture> texture, std::string const & type, unsigned int uvIndex) override;

			void addTexture (std::string const & filename, std::string const & type, unsigned int uvIndex) override;

			void clearTextures () override;

			void setColor (Vector3f const & emitColor, Vector4f const & diffuseColor) override;

			void setSpecular (unsigned int level, float strength) override;

			void setScale (float scale) override;

			void render (Ptr<CameraP> camera, Ptr<EntityP> entity, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

			bool setNeedsResorting () const;

			void resortingDone ();

			bool operator < (ModelP const & model) const;

			static const unsigned int maxLights = 4;

		private:
			class TextureInfo
			{
			public:
				Ptr<TextureP> texture;
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

			float scale;
			int scaleLocation;
		};
	}
}

