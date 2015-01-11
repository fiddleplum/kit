#pragma once

#include "shader.h"
#include "object_cache.h"
#include <kit/singleton.h>

class ShaderCache : public Singleton<ObjectCache<Shader>>
{
};

