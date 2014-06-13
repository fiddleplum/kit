#pragma once

#include <kit/vector.h>

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

