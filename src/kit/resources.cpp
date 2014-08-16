#include "resources.h"
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

		Ptr<Shader> getShader (std::string const & name, std::string code [])
		{
			return _shaderCache->get(name, code);
		}

		Ptr<scene::Model> getBlankModel (std::string const & name)
		{
			return _modelCache->get(name);
		}

		Ptr<scene::Model> getModelFromFile (std::string const & filename)
		{
			return _modelCache->get(filename, filename);
		}

		void initialize ()
		{
			_shaderCache.create();
			_textureCache.create();
			_modelCache.create();
		}

		void finalize ()
		{
			_modelCache.destroy();
			_textureCache.destroy();
			_shaderCache.destroy();
		}
	}
}

