#pragma once

#include "singleton.h"
#include "scene_model.h"
#include "object_cache.h"

class SceneModelCache : public Singleton<ObjectCache<SceneModel>>
{
};

