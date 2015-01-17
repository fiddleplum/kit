#pragma once

#include "ptr.h"
#include <stdexcept>

// A Generic Singleton. Note that calling instance does not lazy-construct the singleton if it does not already exist.
template <typename T>
class Singleton
{
public:
	// Returns the instance. If there is not an instance, returns null.
	static Ptr<T> instance();

	// Creates the instance. If there is already an instance, throws a run_time exception.
	template <typename ...Args> static void createInstance(Args... args);

	// Destroys the instance. If there is not an instance, throws a run_time exception.
	static void destroyInstance();

private:
	static OwnPtr<T> global;
};

// Template Implementation

template <typename T>
Ptr<T> Singleton<T>::instance()
{
	return global;
}

template <typename T> template <typename ...Args>
void Singleton<T>::createInstance(Args... args)
{
	if(global.isValid())
	{
		throw std::exception();
	}
	global.setNew(args...);
}

template <typename T>
void Singleton<T>::destroyInstance()
{
	if(!global.isValid())
	{
		throw std::exception();
	}
	global.setNull();
}

