#pragma once

#include <Vector.h>
#include <Matrix.h>
#include <string>
#include <map>
#include <vector>

class Shader
{
public:
	enum Type
	{
		Vertex,
		Fragment,
		NumTypes
	};

	Shader(std::string const code [NumTypes]);

	~Shader();

	int getUniformLocation(std::string const & name) const;

	int getAttributeLocation(std::string const & name) const;

	void activate();

	static void deactivate();

	static void setUniform(int location, int value);

	static void setUniform(int location, float value);

	static void setUniform(int location, Vector2i value);

	static void setUniform(int location, Vector2f value);

	static void setUniform(int location, Vector3i value);

	static void setUniform(int location, Vector3f value);

	static void setUniform(int location, Vector4i value);

	static void setUniform(int location, Vector4f value);

	static void setUniform(int location, Matrix44f value);

private:
	unsigned int compileShaderObject(std::string const & code, unsigned int type);
	void linkShaderProgram(std::vector<unsigned int> const & shaderObjects);
	void populateVariableLocations();

	unsigned int program;
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;
};

