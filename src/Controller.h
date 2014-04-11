#pragma once

class Event;

class Controller
{
public:
	virtual void handleEvent(Event const &) = 0;
};

