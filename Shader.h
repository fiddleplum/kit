#pragma once

#include <vector>
#include <string>
#include <map>
#include "ShaderComponent.h"

/*
NEW SHADER IDEA:
Instead of having all of these .glsl files and hooking them together, procedurally create the shader based on a Shader::Config class.
This would simplify a lot of things, and still be very flexible. (You could easily map uv indices to texture types, too)

*/

class Shader
{
public:
	class Config
	{
	public:
		/*
			All shaders use uProjection.
		*/
		bool usesWorldView; // requires uWorldView
	};

	Shader(Config const & config);

	// Shader(std::vector<ShaderComponent> const & shaderComponents);

	~Shader();

	int getUniformLocation(std::string const & name);

	int getAttributeLocation(std::string const & name);

	void activate();

	static void deactivate();

private:
	unsigned int createVertexShaderObject(Config const & config);
	unsigned int createFragmentShaderObject(Config const & config);
	unsigned int compileShaderObject(std::code const & code, unsigned int type);
	void linkShaderProgram(unsigned int vertexShaderObject, unsigned int fragmentShaderObject);
	void populateVariableLocations();

	unsigned int program;
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;
};

