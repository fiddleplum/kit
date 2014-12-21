#pragma once

#include <vector>
#include <string>

namespace kit
{
	// Parse content, containing csv-formatted data, and put the result into rows.
	void parseCsv(std::vector<std::vector<std::string>> & rows, std::string const & content);
}

