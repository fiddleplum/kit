#pragma once

#include <kit/resources.h>
#include "texture_p.h"
#include "shader.h"

namespace kit
{
	namespace resources
	{
		Ptr<Shader> getShader (std::string const & name, std::string code []);

		void initialize ();

		void finalize ();
	}

}
