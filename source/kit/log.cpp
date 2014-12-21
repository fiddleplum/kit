#include "log.h"

Log::Log(std::string const & filename)
{
	file.open(filename);
	write("Log started.");
}

Log::~Log()
{
	file.close();
}

void Log::write(std::string const & message)
{
	file << message << std::endl;
}

