#include "texture_p.h"
#include "shader.h"
#include "object_cache.h"
#include "scene/model_p.h"

namespace kit
{
	namespace resources
	{
		OwnPtr<ObjectCache<TextureP>> _textureCache;
		OwnPtr<ObjectCache<Shader>> _shaderCache;
		OwnPtr<ObjectCache<scene::ModelP>> _modelCache;

		UsePtr<Texture> getBlankTexture (std::string const & name, Vector2i size)
		{
			return _textureCache->get(name, size);
		}

		UsePtr<Texture> getTextureFromFile (std::string const & filename)
		{
			return _textureCache->get(filename, filename);
		}

		UsePtr<scene::Model> getBlankModel (std::string const & name)
		{
			return _modelCache->get(name);
		}

		UsePtr<scene::Model> getModelAsSprite (std::string const & name, std::string const & textureFilename, Recti textureCoords)
		{
			return _modelCache->get(name, textureFilename, textureCoords);
		}

		UsePtr<scene::Model> getModelFromFile (std::string const & filename)
		{
			return _modelCache->get(filename, filename);
		}

		UsePtr<Shader> getShader (std::string const & name, std::string code [])
		{
			return _shaderCache->get(name, code);
		}

		void initialize ()
		{
			_shaderCache.set(new ObjectCache<Shader>);
			_textureCache.set(new ObjectCache<TextureP>);
			_modelCache.set(new ObjectCache<scene::ModelP>);
		}

		void finalize ()
		{
			_modelCache.setNull();
			_textureCache.setNull();
			_shaderCache.setNull();
		}
	}
}

