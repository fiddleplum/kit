#pragma once

#include "object_cache.h"

class Texture;
class Shader;

extern OwnPtr<ObjectCache<Texture>> textureCache;
extern OwnPtr<ObjectCache<Shader>> shaderCache;

