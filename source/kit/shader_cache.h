#pragma once

#include "shader.h"
#include "object_cache.h"
#include "singleton.h"

class ShaderCache : public Singleton<ObjectCache<Shader>>
{
};

