#pragma once

#include <vector>
#include <string>
#include <map>
#include "ShaderComponent.h"

class Shader
{
public:
	Shader(std::vector<ShaderComponent> const & shaderComponents);

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

