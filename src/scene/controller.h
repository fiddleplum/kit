#pragma once

namespace kit
{
	class Event;

	namespace scene
	{
		class Controller
		{
		public:
			virtual void handleEvent(Event const &) = 0;
		};
	}
}

