#pragma once

#include "ptr.h"
#include <string>
#include <exception>
#include <map>

template <typename Resource>
class ResourceLoader
{
public:
	// Returns a resource specified by name.
	// If the resource is not already loaded, throws an exception.
	Ptr<Resource> get(std::string const & name) const;

	// Returns true if the resource specified by name is already loaded.
	bool isLoaded(std::string const & name) const;

	// Loads and retursn a resource by calling "new Resource(args...)", and names it.
	// If a loaded resource already has the same name, returns the existing resource.
	// The resource constructor may throw an exception if there are problems loading it.
	template <typename... Args>
	Ptr<Resource> load(std::string const & name, Args... args);

	// Unloads a resource specified by name. If the resource is already not loaded, does nothing.
	void unload(std::string const & name);

	// Unloads all resources.
	void unloadAll();

private:
	std::map<std::string, OwnPtr<Resource>> resources;
};

// Template Implementation

template <typename Resource>
Ptr<Resource> ResourceLoader<Resource>::get(std::string const & name) const
{
	auto it = resources.find(name);
	if(it != resources.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("The resource '" + name + "' is not loaded.");
	}
}

template <typename Resource>
bool ResourceLoader<Resource>::isLoaded(std::string const & name) const
{
	return resources.find(name) != resources.end();
}

template <typename Resource>
template <typename... Args>
void ResourceLoader<Resource>::load(std::string const & name, Args... args)
{
	auto it = resources.find(name);
	if(it == resources.end())
	{
		OwnPtr<Resource> resource = new Resource(args);
		resources.insert(name, resource);
	}
}

template <typename Resource>
void ResourceLoader<Resource>::unload(std::string const & name)
{
	auto it = resources.find(name);
	if(it != resources.end())
	{
		resources.erase(it);
	}
}

template <typename Resource>
void ResourceLoader<Resource>::unloadAll()
{
	resources.clear();
}

