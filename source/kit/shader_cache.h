#pragma once

#include "singleton.h"
#include "shader.h"
#include "object_cache.h"

class ShaderCache : public Singleton<ObjectCache<Shader>>
{
};

