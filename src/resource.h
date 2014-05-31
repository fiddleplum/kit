#pragma once

#include <kit/ptr.h>
#include "resource_manager.h"

namespace kit
{
	class Texture;
	class Shader;

	void startResourceManagers ();
	void finishResourceManagers ();

	// Gets the texture manager.
	Ptr<ResourceManager<Texture>> getTextureManager();

	// Gets the shader manager.
	Ptr<ResourceManager<Shader>> getShaderManager();
}

