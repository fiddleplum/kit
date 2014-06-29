#pragma once

#include "../event.h"

namespace kit
{
	namespace scene
	{
		class Controller
		{
		public:
			virtual void handleEvent(Event const &) = 0;
		};
	}
}

