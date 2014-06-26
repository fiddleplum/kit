#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <string>

namespace kit
{
	class Texture
	{
	public:
		virtual Vector2i getSize () const = 0;

		virtual unsigned char * lockPixels () = 0;

		virtual void unlockPixels () = 0;
	};
}

