#include "log.h"
#include <fstream>

namespace kit
{
	namespace log
	{
		std::ofstream file;

		void initialize ()
		{
			//file.open("log.txt");
			write("Log started.");
			//file.close();
		}

		void finalize ()
		{
			//file.close();
		}

		void write (std::string const & message)
		{
			file.open("log.txt", std::ios::app);
			file << message << std::endl;
			file.close();
		}
	}
}

