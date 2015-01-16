#pragma once

#include <list>
#include <unordered_set>
#include <functional>

template <typename T>
class ObjectVector
{
public:
	// Default constructor.
	ObjectVector();

	// Initializer list constructor.
	ObjectVector(std::initializer_list<T> il);

	// Finds an element based on another type.
	template <typename Y> T & find(Y const & item);

	// Finds an element based on another type.
	template <typename Y> T const & find(Y const & item) const;

	// Adds an item at the end.
	void add(T const & item);

	// Inserts an item before another item.
	template <typename Y> void insertBefore(Y const & beforeItem, T const & item);

	// Removes an item.
	template <typename Y> T const & remove(Y const & item);

	// Removes all items.
	void clear();

	// Processes any removes that have accumulated since the last call.
	void processRemoves();

	// The iterators.
	typename std::list<T>::iterator begin();
	typename std::list<T>::iterator end();
	typename std::list<T>::const_iterator begin() const;
	typename std::list<T>::const_iterator end() const;
	typename std::list<T>::reverse_iterator rbegin();
	typename std::list<T>::reverse_iterator rend();
	typename std::list<T>::const_reverse_iterator rbegin() const;
	typename std::list<T>::const_reverse_iterator rend() const;

private:
	class IteratorHash;
	std::list<T> items;
	std::unordered_set<typename std::list<T>::const_iterator, IteratorHash> removes;
};

// Internal Implementation

template <typename T>
ObjectVector<T>::ObjectVector()
{
}

template <typename T>
ObjectVector<T>::ObjectVector(std::initializer_list<T> il)
{
	for(auto const & item : il)
	{
		items.push_back(item);
	}
}

template <typename T> template <typename Y>
T & ObjectVector<T>::find(Y const & item)
{
	for(auto & t : items)
	{
		if(t == item)
		{
			return t;
		}
	}
	throw std::exception();
}

template <typename T> template <typename Y>
T const & ObjectVector<T>::find(Y const & item) const
{
	for(auto const & t : items)
	{
		if(t == item)
		{
			return t;
		}
	}
	throw std::exception();
}

template <typename T>
void ObjectVector<T>::add(T const & item)
{
	items.push_back(item);
}

template <typename T> template <typename Y>
void ObjectVector<T>::insertBefore(Y const & beforeItem, T const & item)
{
	for(auto it = items.begin(); it != items.end(); ++it)
	{
		if(*it == beforeItem)
		{
			items.insert(it, item);
			return;
		}
	}
	throw std::exception();
}

template <typename T> template <typename Y>
T const & ObjectVector<T>::remove(Y const & item)
{
	for(auto it = items.begin(); it != items.end(); ++it)
	{
		if(*it == item)
		{
			removes.insert(it);
			return *it;
		}
	}
	throw std::exception();
}

template <typename T>
void ObjectVector<T>::clear()
{
	for(auto it = items.begin(); it != items.end(); ++it)
	{
		removes.push_back(it);
	}
}

template <typename T>
void ObjectVector<T>::processRemoves()
{
	for(auto it : removes)
	{
		l.erase(it);
	}
	removes.clear();
}

template <typename T>
typename std::list<T>::iterator ObjectVector<T>::begin()
{
	return items.begin();
}

template <typename T>
typename std::list<T>::iterator ObjectVector<T>::end()
{
	return items.end();
}

template <typename T>
typename std::list<T>::const_iterator ObjectVector<T>::begin() const
{
	return items.cbegin();
}

template <typename T>
typename std::list<T>::const_iterator ObjectVector<T>::end() const
{
	return items.cend();
}

template <typename T>
typename std::list<T>::reverse_iterator ObjectVector<T>::rbegin()
{
	return items.rbegin();
}

template <typename T>
typename std::list<T>::reverse_iterator ObjectVector<T>::rend()
{
	return items.rend();
}

template <typename T>
typename std::list<T>::const_reverse_iterator ObjectVector<T>::rbegin() const
{
	return items.crbegin();
}

template <typename T>
typename std::list<T>::const_reverse_iterator ObjectVector<T>::rend() const
{
	return items.crend();
}

template <typename T>
class ObjectVector<T>::IteratorHash
{
public:
	size_t operator () (typename std::list<T>::const_iterator const & it)
	{
		return (unsigned int)(*it);
	}
};

