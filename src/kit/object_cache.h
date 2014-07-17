#pragma once

#include "ptr.h"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace kit
{
	// Generic object cache.
	// TODO: Uncomment the variadic templated version of get when MSVC++ supports them (and get rid of the existing ones).
	template <typename Object>
	class ObjectCache
	{
	public:
		// Constructor. O(1)
		ObjectCache ();

		// Destructor. Cleans. Throws an exception if it still has any objects. O(number of loaded objects)
		~ObjectCache ();

		// Returns a Ptr of the requested object. O(log number of loaded objects)
		template <typename... Args> Ptr<Object> get (std::string const & key, Args... args);

		// Removes and destroys the objects that aren't referenced anywhere else. O(number of loaded objects).
		void clean ();

		// Gets list of objects in cache by key.
		std::vector<std::string> getObjectKeys ();

	private:
		std::map<std::string, OwnPtr<Object>> _objects;
	};

	// Template Implementations

	template <typename Object>
	ObjectCache<Object>::ObjectCache ()
	{
	}

	template <typename Object>
	ObjectCache<Object>::~ObjectCache ()
	{
		clean();
	}

	template <typename Object>
	template <typename... Args>
	Ptr<Object> ObjectCache<Object>::get (std::string const & key, Args... args)
	{
		auto it = _objects.find(key);
		if(it != _objects.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Object> object;
			try
			{
				object.setNew(args...);
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + key + "': " + e.what());
			}
			_objects[key] = object;
			return object;
		}
	}

	template <typename Object>
	void ObjectCache<Object>::clean ()
	{
		for(auto it = _objects.begin(); it != _objects.end(); )
		{
			if(!it->second.isReferenced())
			{
				it = _objects.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	template <typename Object>
	std::vector<std::string> ObjectCache<Object>::getObjectKeys ()
	{
		std::vector<std::string> keys;
		for(auto const & pair : _objects)
		{
			keys.push_back(pair.first);
		}
		return keys;
	}
}

