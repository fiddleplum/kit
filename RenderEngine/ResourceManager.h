#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <memory>

/// Generic resource manager. Resource must be constructable by Config, and Config must have the < and == operators.
template <typename Resource, typename... Args>
class ResourceManager
{
public:
	/// Constructor. O(1)
	ResourceManager();

	/// Destructor. Cleans. Throws an exception if it still has any resources. O(number of loaded resources)
	~ResourceManager();

	/// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
	std::shared_ptr<Resource> get(Args... args);

	/// Removes and destroys the resources that aren't referenced anywhere else. O(number of loaded resources)
	void clean();

private:
	std::map<std::tuple<Args...>, std::shared_ptr<Resource>> mResources;
};

// Template Implementations

template <typename Resource, typename... Args>
ResourceManager<Resource, Args...>::ResourceManager()
{
}

template <typename Resource, typename... Args>
ResourceManager<Resource, Args...>::~ResourceManager()
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

template <typename Resource, typename... Args>
std::shared_ptr<Resource> ResourceManager<Resource, Args...>::get(Args... args)
{
	auto it = mResources.find(std::tuple<Args...>(args...));
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> resource {std::make_shared<Resource>(args...)};
		mResources[std::tuple<Args...>(args...)] = resource;
		return resource;
	}
}

template <typename Resource, typename... Args>
void ResourceManager<Resource, Args...>::clean()
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

