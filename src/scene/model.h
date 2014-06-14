#pragma once

#include <kit/scene/model.h>
#include <vector>
#include <istream>
#include <set>

namespace kit
{
	class VertexBufferObject;
	class Texture;
	class Shader;

	namespace scene
	{
		class Camera;
		class Frame;

		class Model::P
		{
		public:
			void render (Ptr<Camera> camera, Frame const & frame, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

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

			float scale;
			int scaleLocation;

			friend class Model;
		};
	}
}


