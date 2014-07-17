#pragma once

#include <string>
#include <vector>

namespace kit
{
	namespace fs
	{
		bool isFolder(std::string const & path);

		std::vector<std::string> getFilesInFolder(std::string const & path);
	}
}

