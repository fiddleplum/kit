#pragma once

#include <vector>
#include <string>

class Shader
{
public:
	enum
	{
		Vertex,
		Geometry,
		Fragment,
		NumShaderTypes
	};

	Shader(std::vector<std::string> const & code);
	~Shader();
	
	void activate();
	
	static void deactivate();

private:
	unsigned int program;
};

