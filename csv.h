#pragma once

#include <vector>
#include <string>

//! Will parse \b content, containing csv-formatted data, and put the result into \b rows.
void parseCsv (std::vector<std::vector<std::string>> & rows, std::string const & content);

