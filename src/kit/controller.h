#pragma once

#include "event.h"

namespace kit
{
	class Controller
	{
	public:
		virtual void handleEvent(Event const &) = 0;
	};
}

