#pragma once

#include <kit/ptr.h>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// Generic object cache.
template <typename Object>
class ObjectCache
{
public:
	// Constructor. O(1)
	ObjectCache();

	// Destructor. Cleans. Throws an exception if it still has any objects. O(number of loaded objects)
	~ObjectCache();

	// Returns true if an object with the given name is in the cache. O(log number of loaded objects)
	bool has(std::string const & name) const;

	// Returns a Ptr of the object of the given name. If an object with the given name isn't already in the cache, throws an exception. O(log number of loaded objects)
	Ptr<Object> get(std::string const & name) const;

	// Returns a Ptr of the object of the given name. If an object with the given name isn't already in the cache, loads the object. O(log number of loaded objects)
	template <typename... Args> Ptr<Object> load(std::string const & name, Args... args);

	// Removes and destroys the objects that aren't referenced outside of the cache. O(number of loaded objects).
	void clean();

	// Gets a list of objects in the cache by name.
	std::vector<std::string> getObjectKeys();

private:
	std::map<std::string, OwnPtr<Object>> objects;
};

// Template Implementations

template <typename Object>
ObjectCache<Object>::ObjectCache()
{
}

template <typename Object>
ObjectCache<Object>::~ObjectCache()
{
	clean();
	if(!objects.empty())
	{
		std::string message = "There are still objects referenced:\n";
		for(auto const & pair : objects)
		{
			message += pair.first + "\n";
		}
		throw std::runtime_error(message);
	}
}

template <typename Object>
bool ObjectCache<Object>::has(std::string const & name) const
{
	return objects.find(name) != objects.end();
}

template <typename Object>
Ptr<Object> ObjectCache<Object>::get(std::string const & name) const
{
	auto it = objects.find(name);
	if(it != objects.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("'" + name + "' was not found in the cache.");
	}
}

template <typename Object>
template <typename... Args>
Ptr<Object> ObjectCache<Object>::load(std::string const & name, Args... args)
{
	auto it = objects.find(name);
	if(it != objects.end())
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
			throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
		}
		objects[name] = object;
		return object;
	}
}

template <typename Object>
void ObjectCache<Object>::clean()
{
	for(auto it = objects.begin(); it != objects.end();)
	{
		if(!it->second.isReferenced())
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

template <typename Object>
std::vector<std::string> ObjectCache<Object>::getObjectKeys()
{
	std::vector<std::string> keys;
	for(auto const & pair : objects)
	{
		keys.push_back(pair.first);
	}
	return keys;
}

