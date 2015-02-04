#pragma once

#include "object_cache.h"

class Texture;
class Shader;
class Font;
class SceneModel;

extern OwnPtr<ObjectCache<Texture>> textureCache;
extern OwnPtr<ObjectCache<Shader>> shaderCache;
extern OwnPtr<ObjectCache<Font>> fontCache;
extern OwnPtr<ObjectCache<SceneModel>> sceneModelCache;

