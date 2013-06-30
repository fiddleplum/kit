#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <memory>

///******* NOTE: CAN ResourceManager take a Args... instead of Config, and can this Args... be converted into a tuple?
///******* OTHERWISE: Put ResourceManager<Texture,Texture::Config> in Texture.h as a shared_ptr. App.cpp (not SDLApp.cpp) will instantiate and destroy them.

/// Generic resource manager. Resource must be constructable by Config, and Config must have the < and == operators.
template <typename Resource, typename Config>
class ResourceManager
{
public:
	/// Constructor. O(1)
	ResourceManager();

	/// Destructor. Cleans. Throws an exception if it still has any resources. O(number of loaded resources)
	~ResourceManager();

	/// Returns a shared_ptr of the requested resource. O(log number of loaded resources)
	std::shared_ptr<Resource> get(Config const & config);

	/// Removes and destroys the resources that aren't referenced anywhere else. O(number of loaded resources)
	void clean();

private:
	std::map<Config, std::shared_ptr<Resource>> mResources;
};

// Template Implementations

template <typename Resource, typename Config>
ResourceManager<Resource, Config>::ResourceManager()
{
}

template <typename Resource, typename Config>
ResourceManager<Resource, Config>::~ResourceManager()
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

template <typename Resource, typename Config>
std::shared_ptr<Resource> ResourceManager<Resource, Config>::get(Config const & config)
{
	auto it = mResources.find(config);
	if(it != mResources.end())
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<Resource> resource {std::make_shared<Resource>(config)};
		mResources[config] = resource;
		return resource;
	}
}

template <typename Resource, typename Config>
void ResourceManager<Resource, Config>::clean()
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

