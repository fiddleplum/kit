#pragma once

#include <string>

namespace kit
{
	namespace log
	{
		void initialize ();

		void finalize ();

		void write (std::string const & message);
	}
}

