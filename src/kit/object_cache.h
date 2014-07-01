#pragma once

#include "ptr.h"
#include <map>
#include <stdexcept>
#include <string>

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
		//template <typename... Args> Ptr<Object> get (std::string const & key, Args... args);

		// Returns a Ptr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		Ptr<Object> get (std::string const & key);

		// Returns a Ptr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		template <typename T1> Ptr<Object> get (std::string const & key, T1 const & arg1);

		// Returns a Ptr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		template <typename T1, typename T2> Ptr<Object> get (std::string const & key, T1 const & arg1, T2 const & arg2);

		// Returns a Ptr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		template <typename T1, typename T2, typename T3> Ptr<Object> get (std::string const & key, T1 const & arg1, T2 const & arg2, T3 const & arg3);

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

	//template <typename Object>
	//template <typename... Args>
	//Ptr<Object> ObjectCache<Object>::get (std::string const & key, Args... args)
	//{
	//	auto it = _objects.find(key);
	//	if(it != _objects.end())
	//	{
	//		return it->second;
	//	}
	//	else
	//	{
	//		OwnPtr<Object> object;
	//		try
	//		{
	//			object.set(new Object (args...));
	//		}
	//		catch(std::runtime_error const & e)
	//		{
	//			throw std::runtime_error("Error while constructing '" + key + "': " + e.what());
	//		}
	//		_objects[key] = object;
	//		return object;
	//	}
	//}

	template <typename Object>
	Ptr<Object> ObjectCache<Object>::get (std::string const & key)
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
				object.set(new Object);
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
	template <typename T1>
	Ptr<Object> ObjectCache<Object>::get (std::string const & key, T1 const & arg1)
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
				object.set(new Object (arg1));
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
	template <typename T1, typename T2>
	Ptr<Object> ObjectCache<Object>::get (std::string const & key, T1 const & arg1, T2 const & arg2)
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
				object.set(new Object (arg1, arg2));
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
	template <typename T1, typename T2, typename T3>
	Ptr<Object> ObjectCache<Object>::get (std::string const & key, T1 const & arg1, T2 const & arg2, T3 const & arg3)
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
				object.set<Object>(new Object (arg1, arg2, arg3));
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

