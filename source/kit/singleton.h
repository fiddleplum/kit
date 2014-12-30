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
	static void createInstance();

	// Destroys the instance. If there is not an instance, throws a run_time exception.
	static void destroyInstance();

private:
	static OwnPtr<T> t;
};

// Template Implementation

template <typename T>
Ptr<T> Singleton<T>::instance()
{
	return t;
}

template <typename T>
void Singleton<T>::createInstance()
{
	if(t.isValid())
	{
		throw std::exception();
	}
	t.create();
}

template <typename T>
void Singleton<T>::destroyInstance()
{
	if(!t.isValid())
	{
		throw std::exception();
	}
	t.destroy();
}

