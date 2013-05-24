#pragma once

#include <vector>
#include <string>
#include <map>

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
	int getUniformLocation(std::string const & name);
	int getAttributeLocation(std::string const & name);
	void activate();
	static void deactivate();

private:
	unsigned int program;
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;
};

