#pragma once

#include <string>
#include <fstream>

class Log
{
public:
	Log(std::string const & filename = "log.txt");

	~Log();

	void write(std::string const & message);

private:
	std::ofstream file;
};

