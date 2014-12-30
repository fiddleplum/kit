#pragma once

#include <type_traits>
#include <string>

// A class that you can inherit from that allows for a non-dynamically allocated pimpl idiom.
// Just give it the size of your pimpl class.
template <typename Type, unsigned int size, unsigned int align>
class Pimpl
{
public:
	Pimpl()

protected:
	Type const & p() const;
	Type & p();

private:
	std::aligned_storage<size, align> storage;
};

template <typename Type, unsigned int size, unsigned int align>
Pimpl<Type, size, align>::Pimpl()
{
	if(size != sizeof(Type) || align != alignof(Type))
	{
		throw std::runtime_error("Size or align is incorrect. Should be" + std::to_string(sizeof(Type)) + " and " + std::to_string(alignof(Type)));
	}
}

template <typename Type, unsigned int size, unsigned int align>
Type const & Pimpl<Type, size, align>::p() const
{
	return static_cast<Type>(storage);
}

template <typename Type, unsigned int size, unsigned int align>
Type & Pimpl<Type, size, align>::p()
{
	return static_cast<Type>(storage);
}

