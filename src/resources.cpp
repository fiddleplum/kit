#include <kit/texture.h>
#include <kit/shader.h>"
#include <kit/scene/model.h>
#include <kit/resources.h>
#include "object_cache.h"

namespace kit
{
	namespace resources
	{
		OwnPtr<ObjectCache<Texture>> _textureCache;
		OwnPtr<ObjectCache<Shader>> _shaderCache;
		OwnPtr<ObjectCache<scene::Model>> _modelCache;

		Ptr<Texture> getBlankTexture (std::string const & name, Vector2i size)
		{
			return _textureCache->get(name, size);
		}

		Ptr<Texture> getTextureFromFile (std::string const & filename)
		{
			return _textureCache->get(filename, filename);
		}

		Ptr<scene::Model> getBlankModel (std::string const & name)
		{
			return _modelCache->get(name);
		}

		Ptr<scene::Model> getModelAsSprite (std::string const & name, std::string const & textureFilename, Recti textureCoords)
		{
			return _modelCache->get(name, textureFilename, textureCoords);
		}

		Ptr<scene::Model> getModelFromFile (std::string const & filename)
		{
			return _modelCache->get(filename, filename);
		}

		Ptr<Shader> getShader (std::string const & name, std::string code [])
		{
			return _shaderCache->get(name, code);
		}

		void initialize ()
		{
			_shaderCache.set(new ObjectCache<Shader>);
			_textureCache.set(new ObjectCache<Texture>);
			_modelCache.set(new ObjectCache<scene::Model>);
		}

		void finalize ()
		{
			_modelCache.setNull();
			_textureCache.setNull();
			_shaderCache.setNull();
		}
	}
}

