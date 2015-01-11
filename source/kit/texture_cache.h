#pragma once

#include <kit/singleton.h>
#include "texture.h"
#include "object_cache.h"

class TextureCache : public Singleton<ObjectCache<Texture>>
{
};

