#pragma once

class Event;

namespace Scene2D
{
	class Controller
	{
	public:
		virtual void handleEvent(Event const &) = 0;
	};
}

