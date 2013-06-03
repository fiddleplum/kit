#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <memory>

/// Generic resource manager.
template <typename Resource>
class ResourceManager
{
public:
	/// Constructor. O(1)
	ResourceManager();

	/// Destructor. Cleans. Throws an exception if it still has any resources. O(n)
	~ResourceManager();

	/// Returns a shared_ptr of the requested resource. O(log n)
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

