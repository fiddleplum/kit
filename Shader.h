#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <vector>
#include <string>
#include <map>

class Shader
{
public:
	class Config
	{
	public:
		/*
			All shaders use uProjection.
		*/
		bool usesWorldView = false; // requires uWorldView
		bool usesVertexNormal = false; // requires aNormal
		bool usesVertexColor = false; // requires aColor
		
	};

	Shader(std::string const & vertexCode, std::string const & fragmentCode);

	Shader(Config const & config);

	// Shader(std::vector<ShaderComponent> const & shaderComponents);

	~Shader();

	int getUniformLocation(std::string const & name);

	int getAttributeLocation(std::string const & name);

	void setUniform(int location, int value) const;

	void setUniform(int location, float value) const;

	void setUniform(int location, Vector2i value) const;

	void setUniform(int location, Vector2f value) const;

	void setUniform(int location, Vector3i value) const;

	void setUniform(int location, Vector3f value) const;

	void setUniform(int location, Vector4i value) const;

	void setUniform(int location, Vector4f value) const;

	void setUniform(int location, Matrix44f value) const;

	void activate();

	static void deactivate();

private:
	unsigned int createVertexShaderObject(Config const & config);
	unsigned int createFragmentShaderObject(Config const & config);
	unsigned int compileShaderObject(std::string const & code, unsigned int type);
	void linkShaderProgram(unsigned int vertexShaderObject, unsigned int fragmentShaderObject);
	void populateVariableLocations();

	unsigned int program;
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;
};

