#pragma once

#include <kit/ptr.h>
#include <map>
#include <stdexcept>
#include <string>

namespace kit
{
	// Generic object cache.
	// TODO: Uncomment the variadic templated version of get when MSVC++ supports them (and get rid of the existing ones).
	template <typename Resource>
	class ObjectCache
	{
	public:
		// Constructor. O(1)
		ObjectCache ();

		// Destructor. Cleans. Throws an exception if it still has any objects. O(number of loaded objects)
		~ObjectCache ();

		// Returns a UsePtr of the requested object. O(log number of loaded objects)
		//template <typename... Args> UsePtr<Resource> get (std::string const & name, Args... args);

		// Returns a UsePtr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		UsePtr<Resource> get (std::string const & name);

		// Returns a UsePtr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		template <typename T1> UsePtr<Resource> get (std::string const & name, T1 const & arg1);

		// Returns a UsePtr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		template <typename T1, typename T2> UsePtr<Resource> get (std::string const & name, T1 const & arg1, T2 const & arg2);

		// Returns a UsePtr of the requested object. O(log number of loaded objects). This can be removed when the variadic template works.
		template <typename T1, typename T2, typename T3> UsePtr<Resource> get (std::string const & name, T1 const & arg1, T2 const & arg2, T3 const & arg3);

		// Removes and destroys the objects that aren't referenced anywhere else. O(number of loaded objects)
		void clean ();

	private:
		std::map<std::string, OwnPtr<Resource>> _objects;
	};

	// Template Implementations

	template <typename Resource>
	ObjectCache<Resource>::ObjectCache ()
	{
	}

	template <typename Resource>
	ObjectCache<Resource>::~ObjectCache ()
	{
		clean();
		if(!_objects.empty())
		{
			std::string message (std::string() + "At least one " + typeid(Resource).name() + " not released:\n");
			for(auto const & pair : _objects)
			{
				message += "\t" + pair.first + "\n";
			}
			throw std::runtime_error(message);
		}
	}

	//template <typename Resource>
	//template <typename... Args>
	//UsePtr<Resource> ObjectCache<Resource>::get (std::string const & name, Args... args)
	//{
	//	auto it = _objects.find(name);
	//	if(it != _objects.end())
	//	{
	//		return it->second;
	//	}
	//	else
	//	{
	//		OwnPtr<Resource> object;
	//		try
	//		{
	//			object.set(new Resource (args...));
	//		}
	//		catch(std::runtime_error const & e)
	//		{
	//			throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
	//		}
	//		_objects[name] = object;
	//		return object;
	//	}
	//}

	template <typename Resource>
	UsePtr<Resource> ObjectCache<Resource>::get (std::string const & name)
	{
		auto it = _objects.find(name);
		if(it != _objects.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> object;
			try
			{
				object.set(new Resource);
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			_objects[name] = object;
			return object;
		}
	}

	template <typename Resource>
	template <typename T1>
	UsePtr<Resource> ObjectCache<Resource>::get (std::string const & name, T1 const & arg1)
	{
		auto it = _objects.find(name);
		if(it != _objects.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> object;
			try
			{
				object.set(new Resource (arg1));
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			_objects[name] = object;
			return object;
		}
	}

	template <typename Resource>
	template <typename T1, typename T2>
	UsePtr<Resource> ObjectCache<Resource>::get (std::string const & name, T1 const & arg1, T2 const & arg2)
	{
		auto it = _objects.find(name);
		if(it != _objects.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> object;
			try
			{
				object.set(new Resource (arg1, arg2));
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			_objects[name] = object;
			return object;
		}
	}

	template <typename Resource>
	template <typename T1, typename T2, typename T3>
	UsePtr<Resource> ObjectCache<Resource>::get (std::string const & name, T1 const & arg1, T2 const & arg2, T3 const & arg3)
	{
		auto it = _objects.find(name);
		if(it != _objects.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> object;
			try
			{
				object.set<Resource>(new Resource (arg1, arg2, arg3));
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			_objects[name] = object;
			return object;
		}
	}

	template <typename Resource>
	void ObjectCache<Resource>::clean ()
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
}

