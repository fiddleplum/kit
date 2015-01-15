#pragma once

#include "coord.h"
#include "matrix.h"
#include <string>
#include <map>
#include <vector>

class Shader
{
public:
	enum CodeType
	{
		Vertex, Fragment, NumCodeTypes
	};

	Shader(std::string const code[NumCodeTypes]);

	~Shader();

	int getUniformLocation(std::string const & name) const;

	int getAttributeLocation(std::string const & name) const;

	void activate();

	static void deactivate();

	static void setUniform(int location, int value);

	static void setUniform(int location, float value);

	static void setUniform(int location, Coord2i value);

	static void setUniform(int location, Coord2f value);

	static void setUniform(int location, Coord3i value);

	static void setUniform(int location, Coord3f value);

	static void setUniform(int location, Coord4i value);

	static void setUniform(int location, Coord4f value);

	static void setUniform(int location, Matrix33f const & value);

	static void setUniform(int location, Matrix44f const & value);

	static void setUniform(int location, Coord3f const * value, unsigned int count);

	static void setUniform(int location, Coord2f const * value, unsigned int count);

private:
	static unsigned int compileShaderObject(CodeType type, std::string const & code);
	static unsigned int linkShaderProgram(std::vector<unsigned int> const & shaderObjects);
	void populateVariableLocations();

	unsigned int program;
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;
};

