#pragma once

#include "../ptr.h"
#include "../noncopyable.h"
#include "../vector.h"
#include "../rect.h"
#include "../texture.h"
#include "camera.h"
#include "light.h"
#include <string>
#include <vector>

namespace kit
{
	class Shader;
	class VertexBufferObject;

	namespace scene
	{
		class Model : Noncopyable
		{
		public:
			// Create a blank model.
			Model ();

			// Load from a file.
			Model (std::string const & filename);

			void setVertexFormat(bool hasNormal, bool hasTangent, bool hasColor, unsigned int numVertexUVs);

			void setVertices(void const * vertices, unsigned int numBytes);

			void setNumIndicesPerPrimitive(unsigned int num);

			void setIndices(unsigned int const * indices, unsigned int numIndices);

			Ptr<Texture> getTexture(unsigned int textureIndex) const;

			void addTexture(Ptr<Texture> texture, std::string const & type, unsigned int uvIndex);

			void addTexture(std::string const & filename, std::string const & type, unsigned int uvIndex);

			void clearTextures();

			void setColor(Vector3f const & emitColor, Vector4f const & diffuseColor);

			void setSpecular(unsigned int level, float strength);

			float getScale() const;

			void setScale(float scale);

			void render (Ptr<Camera> camera, Ptr<Entity> entity, std::vector<Vector3f> const & lightPositions, std::vector<Vector3f> const & lightColors) const;

			static const unsigned int maxLights = 4;

			bool needsResorting () const;

			void resortingDone ();

			bool operator < (Model const & model) const;

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

			Vector3f _emitColor;
			int _emitColorLocation;
			Vector4f _diffuseColor;
			int _diffuseColorLocation;
			unsigned int _specularLevel;
			int _specularLevelLocation;
			float _specularStrength;
			int _specularStrengthLocation;

			int _lightPositionsLocation;
			int _lightColorsLocation;

			std::vector<TextureInfo> _textureInfos;

			bool _vertexHasNormal;
			bool _vertexHasTangent;
			bool _vertexHasColor;
			unsigned int _numVertexUVs;
			unsigned int _numBytesPerVertex;
			OwnPtr<VertexBufferObject> _vertexBufferObject;

			Ptr<Shader> _shader;
			bool _sorted;

			int _projectionLocation;
			int _worldViewLocation;

			float _scale;
			int _scaleLocation;
		};
	}
}

