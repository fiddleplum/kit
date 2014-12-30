#pragma once

#include <memory>

// A templated Pimpl class. All you need to do is inherit from it as "class T : publi Pimpl<T>" and define a "class Pimpl<T>::P"
//   with your private data and non-virtual private functions in it.
template <typename T>
class Pimpl
{
public:
	// Constructor.
	Pimpl();

	// Destructor.
	virtual ~Pimpl();

protected:
	class P;
	std::unique_ptr<P> p;
};

// Template Implementation

template <typename T>
Pimpl<T>::Pimpl()
{
	p.reset(new P);
}

template <typename T>
Pimpl<T>::~Pimpl()
{
	p.reset(nullptr);
}

