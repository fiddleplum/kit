#pragma once

#include "scene_model.h"
#include "object_cache.h"
#include <kit/singleton.h>

class SceneModelCache : public Singleton<ObjectCache<SceneModel>>
{
};

