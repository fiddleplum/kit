#pragma once

class Event;

namespace Scene
{
	class Controller
	{
	public:
		virtual void handleEvent(Event const &) = 0;
	};
}

