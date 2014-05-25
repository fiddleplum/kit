#pragma once

#include "string_util.h"
#include <unordered_map>
#include <stack>
#include <stdexcept>

namespace kit
{
	/*
	A container where you put objects in and then refer to them by a key that is given back.
	Same time complexity as an unordered_map.
	*/

	template <typename T>
	class Storage
	{
	public:
		Storage();
		unsigned int add(T t); // invalidates iterators
		void remove(unsigned int id); // invalidates iterators
		T get(unsigned int id) const;

		bool empty();
		unsigned int size();
		void clear();

		typename std::unordered_map<unsigned int, T>::iterator begin();
		typename std::unordered_map<unsigned int, T>::iterator end();
		typename std::unordered_map<unsigned int, T>::iterator cbegin() const;
		typename std::unordered_map<unsigned int, T>::iterator cend() const;

	private:
		std::unordered_map<unsigned int, T> map;
		std::stack<unsigned int> freeIds;
		unsigned int nextFreeId;
	};

	// Template Implementation

	template <typename T>
	Storage<T>::Storage()
	{
		nextFreeId = 0;
	}

	template <typename T>
	unsigned int Storage<T>::add(T t)
	{
		unsigned int id = 0;
		if(freeIds.empty())
		{
			id = nextFreeId;
			nextFreeId++;
		}
		else
		{
			id = freeIds.top();
			freeIds.pop();
		}
		map[id] = t;
		return id;
	}

	template <typename T>
	void Storage<T>::remove(unsigned int id)
	{
		unsigned int numErased = map.erase(id);
		if(numErased == 1)
		{
			freeIds.push(id);
		}
		else
		{
			throw std::out_of_range("In Storage::remove, an invalid id: " + std::to_string(id));
		}
	}

	template <typename T>
	T Storage<T>::get(unsigned int id) const
	{
		auto it = map.find(id);
		if(it != map.end())
		{
			return it->second;
		}
		else
		{
			throw std::out_of_range("In Storage::get, an invalid id: " + std::to_string(id));
		}
	}

	template <typename T>
	bool Storage<T>::empty()
	{
		return map.empty();
	}

	template <typename T>
	unsigned int Storage<T>::size()
	{
		return map.size();
	}

	template <typename T>
	void Storage<T>::clear()
	{
		map.clear();
		freeIds.clear();
		nextFreeId = 0;
	}

	template <typename T>
	typename std::unordered_map<unsigned int, T>::iterator Storage<T>::begin()
	{
		return map.begin();
	}

	template <typename T>
	typename std::unordered_map<unsigned int, T>::iterator Storage<T>::end()
	{
		return map.end();
	}

	template <typename T>
	typename std::unordered_map<unsigned int, T>::iterator Storage<T>::cbegin() const
	{
		return map.cbegin();
	}

	template <typename T>
	typename std::unordered_map<unsigned int, T>::iterator Storage<T>::cend() const
	{
		return map.cend();
	}
}

