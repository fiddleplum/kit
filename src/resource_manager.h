#pragma once

#include <kit/ptr.h>
#include <map>
#include <stdexcept>
#include <string>
#include <memory>

namespace kit
{
	// Generic resource manager.
	// TODO: Uncomment the variadic templated version of get when MSVC++ supports them (and get rid of the existing ones).
	template <typename Resource>
	class ResourceManager
	{
	public:
		// Constructor. O(1)
		ResourceManager ();

		// Destructor. Cleans. Throws an exception if it still has any resources. O(number of loaded resources)
		~ResourceManager ();

		// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
		//template <typename... Args> Ptr<Resource> get (std::string const & name, Args... args);

		// Returns a shared_ptr of the requested resource. O(log number of loaded resources). This can be removed when the variadic template works.
		Ptr<Resource> get (std::string const & name);

		// Returns a shared_ptr of the requested resource. O(log number of loaded resources). This can be removed when the variadic template works.
		template <typename T1> Ptr<Resource> get (std::string const & name, T1 const & arg1);

		// Returns a shared_ptr of the requested resource. O(log number of loaded resources). This can be removed when the variadic template works.
		template <typename T1, typename T2> Ptr<Resource> get (std::string const & name, T1 const & arg1, T2 const & arg2);

		// Returns a shared_ptr of the requested resource. O(log number of loaded resources). This can be removed when the variadic template works.
		template <typename T1, typename T2, typename T3> Ptr<Resource> get (std::string const & name, T1 const & arg1, T2 const & arg2, T3 const & arg3);

		// Removes and destroys the resources that aren't referenced anywhere else. O(number of loaded resources)
		void clean ();

	private:
		std::map<std::string, OwnPtr<Resource>> mResources;
	};

	// Template Implementations

	template <typename Resource>
	ResourceManager<Resource>::ResourceManager ()
	{
	}

	template <typename Resource>
	ResourceManager<Resource>::~ResourceManager ()
	{
		clean();
		if(!mResources.empty())
		{
			std::string message (std::string() + "At least one " + typeid(Resource).name() + " not released:\n");
			for(auto const & pair : mResources)
			{
				message += "\t" + pair.first + "\n";
			}
			throw std::runtime_error(message);
		}
	}

	//template <typename Resource>
	//template <typename... Args>
	//Ptr<Resource> ResourceManager<Resource>::get (std::string const & name, Args... args)
	//{
	//	auto it = mResources.find(name);
	//	if(it != mResources.end())
	//	{
	//		return it->second;
	//	}
	//	else
	//	{
	//		OwnPtr<Resource> resource;
	//		try
	//		{
	//			resource.set(new Resource (args...));
	//		}
	//		catch(std::runtime_error const & e)
	//		{
	//			throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
	//		}
	//		mResources[name] = resource;
	//		return resource;
	//	}
	//}

	template <typename Resource>
	Ptr<Resource> ResourceManager<Resource>::get (std::string const & name)
	{
		auto it = mResources.find(name);
		if(it != mResources.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> resource;
			try
			{
				resource.set(new Resource);
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			mResources[name] = resource;
			return resource;
		}
	}

	template <typename Resource>
	template <typename T1>
	Ptr<Resource> ResourceManager<Resource>::get (std::string const & name, T1 const & arg1)
	{
		auto it = mResources.find(name);
		if(it != mResources.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> resource;
			try
			{
				resource.set(new Resource (arg1));
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			mResources[name] = resource;
			return resource;
		}
	}

	template <typename Resource>
	template <typename T1, typename T2>
	Ptr<Resource> ResourceManager<Resource>::get (std::string const & name, T1 const & arg1, T2 const & arg2)
	{
		auto it = mResources.find(name);
		if(it != mResources.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> resource;
			try
			{
				resource.set(new Resource (arg1, arg2));
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			mResources[name] = resource;
			return resource;
		}
	}

	template <typename Resource>
	template <typename T1, typename T2, typename T3>
	Ptr<Resource> ResourceManager<Resource>::get (std::string const & name, T1 const & arg1, T2 const & arg2, T3 const & arg3)
	{
		auto it = mResources.find(name);
		if(it != mResources.end())
		{
			return it->second;
		}
		else
		{
			OwnPtr<Resource> resource;
			try
			{
				resource.set<Resource>(new Resource (arg1, arg2, arg3));
			}
			catch(std::runtime_error const & e)
			{
				throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
			}
			mResources[name] = resource;
			return resource;
		}
	}

	template <typename Resource>
	void ResourceManager<Resource>::clean ()
	{
		for(auto it = mResources.begin(); it != mResources.end(); )
		{
			if(!it->second.isReferenced())
			{
				it = mResources.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

