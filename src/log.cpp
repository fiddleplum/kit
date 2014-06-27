#include "log_p.h"
#include <fstream>

namespace kit
{
	namespace log
	{
		std::fstream file;

		void initialize ()
		{
			file.open("log.txt", std::ios::out);
			write("Log started.");
		}

		void finalize ()
		{
			file.close();
		}

		void write (std::string const & message)
		{
			file << message << std::endl;
		}
	}
}

