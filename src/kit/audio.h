#pragma once

#include <string>

namespace kit
{
	namespace audio
	{
		// Called by app
		void initialize();

		// Called by app
		void finalize();

		// Play a file
		void play(std::string const & filename);
	};
}

