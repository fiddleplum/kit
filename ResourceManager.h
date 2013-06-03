#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <memory>

/// Generic resource manager. Configuration must be explicitly castable to a string, and must have the < operator.
template <typename Resource>
class ResourceManager
{
public:
	/// Constructor. O(1)
	ResourceManager();

	/// Destructor. Throws an exception if it still has any resources after a cleaning. O(n)
	~ResourceManager();

	/// Returns a valid shared_ptr if the manager has the resource, otherwise a null shared_ptr. O(log n)
	template <typename ... Args>
	std::shared_ptr<Resource> get(std::string const & name, Args ... args);
	
	/// Removes and destroys the resources that aren't referenced anywhere else. O(n)
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
		std::string message {"Resources not released:\n"};
		for(auto const & pair : mResources)
		{
			message += "  " + (std::string)pair.first + "\n";
		}
		throw std::runtime_error(message);
	}
}

template <typename Resource> template <typename ... Args>
std::shared_ptr<Resource> ResourceManager<Resource>::get(std::string const & name, Args ... args)
{
	auto it = mResources.find(name);
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> pointer {std::make_shared<Resource>(args ...)};
		mResources[name] = pointer;
		return pointer;
	}
}

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

