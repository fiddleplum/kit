#pragma once

#include "app.h"
#include <map>
#include <stdexcept>
#include <string>
#include <memory>

/// Generic resource manager.
/// TODO: Uncomment the variadic templated version of get when MSVC++ supports them (and get rid of the existing ones).
template <typename Resource>
class ResourceManager
{
public:
	/// Constructor. O(1)
	ResourceManager();

	/// Destructor. Cleans. Throws an exception if it still has any resources. O(number of loaded resources)
	~ResourceManager();

	/// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
#ifndef _MSC_VER
	template <typename... Args>
	std::shared_ptr<Resource> get(std::string const & name, Args... args);
#else
	/// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
	template <typename T1>
	std::shared_ptr<Resource> get(std::string const & name, T1 const & arg1);

	/// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
	template <typename T1, typename T2>
	std::shared_ptr<Resource> get(std::string const & name, T1 const & arg1, T2 const & arg2);

	/// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
	template <typename T1, typename T2, typename T3>
	std::shared_ptr<Resource> get(std::string const & name, T1 const & arg1, T2 const & arg2, T3 const & arg3);
#endif

	/// Removes and destroys the resources that aren't referenced anywhere else. O(number of loaded resources)
	void clean();

private:
	std::map<std::string, std::shared_ptr<Resource>> mResources;
};

// Template Implementations

template <typename Resource>
ResourceManager<Resource>::ResourceManager()
{
}

template <typename Resource>
ResourceManager<Resource>::~ResourceManager()
{
	clean();
	if(!mResources.empty())
	{
		std::string message (std::string() + "At least one " + typeid(Resource).name() + " not released:\n");
		for(auto const & pair : mResources)
		{
			message += "\t" + pair.first + "\n";
		}
		App::showMessage(message);
	}
}

#ifndef _MSC_VER
template <typename Resource>
template <typename... Args>
std::shared_ptr<Resource> ResourceManager<Resource>::get(std::string const & name, Args... args)
{
	auto it = mResources.find(name);
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> resource;
		try
		{
			resource = std::make_shared<Resource>(args...);
		}
		catch(std::runtime_error const & e)
		{
			throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
		}
		mResources[name] = resource;
		return resource;
	}
}
#else
template <typename Resource>
template <typename T1>
std::shared_ptr<Resource> ResourceManager<Resource>::get(std::string const & name, T1 const & arg1)
{
	auto it = mResources.find(name);
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> resource;
		try
		{
			resource = std::make_shared<Resource>(arg1);
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
std::shared_ptr<Resource> ResourceManager<Resource>::get(std::string const & name, T1 const & arg1, T2 const & arg2)
{
	auto it = mResources.find(name);
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> resource;
		try
		{
			resource = std::make_shared<Resource>(arg1, arg2);
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
std::shared_ptr<Resource> ResourceManager<Resource>::get(std::string const & name, T1 const & arg1, T2 const & arg2, T3 const & arg3)
{
	auto it = mResources.find(name);
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> resource;
		try
		{
			resource = std::make_shared<Resource>(arg1, arg2, arg3);
		}
		catch(std::runtime_error const & e)
		{
			throw std::runtime_error("Error while constructing '" + name + "': " + e.what());
		}
		mResources[name] = resource;
		return resource;
	}
}
#endif

template <typename Resource>
void ResourceManager<Resource>::clean()
{
	for(auto it = mResources.begin(); it != mResources.end(); )
	{
		if(it->second.unique())
		{
			it = mResources.erase(it);
		}
		else
		{
			it++;
		}
	}
}

