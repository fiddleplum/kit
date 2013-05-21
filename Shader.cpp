#include "Shader.h"

#include "OpenGL.h"
#include <cassert>
#include <stdexcept>

Shader::Shader(std::vector<std::string> const & code)
{
	assert(code.size() == 3);

	bool shaderValid [NumShaderTypes];

	GLuint shaders[NumShaderTypes];
	for(unsigned int type = 0; type < NumShaderTypes; type++)
	{
		// Does this type have any code with it?
		if(code.empty())
		{
			shaderValid[type] = false;
			continue;
		}
		else
		{
			shaderValid[type] = true;
		}

		// Set the OpenGL type.
		GLenum glType;
		switch(type)
		{
		case Vertex:
			glType = GL_VERTEX_SHADER; break;
		case Geometry:
			glType = GL_GEOMETRY_SHADER; break;
		case Fragment:
			glType = GL_FRAGMENT_SHADER; break;
		}

		// Create the shader objects.
		shaders[type] = glCreateShader(type);
		char const * shaderCode = code[type].c_str();
		GLint shaderCodeSize = code[type].size();
		glShaderSource(shaders[type], 1, &shaderCode, &shaderCodeSize);
		glCompileShader(shaders[type]);
		GLint good;
		glGetShaderiv(shaders[type], GL_COMPILE_STATUS, &good);
		if(good == GL_FALSE)
		{
			GLint logLength;
			std::string log;
			glGetShaderiv(shaders[type], GL_INFO_LOG_LENGTH, &logLength);
			log.resize(logLength);
			glGetShaderInfoLog(shaders[type], logLength, 0, &log[0]);
			std::string typeString;
			switch(type)
			{
			case Vertex:
				typeString = "vertex"; break;
			case Geometry:
				typeString = "geometry"; break;
			case Fragment:
				typeString = "fragment"; break;
			}
			throw std::runtime_error("Error compiling " + typeString + " shader: " + log);
		}
	}

	// Create the shader program.
	program = glCreateProgram();
	for(unsigned int type = 0; type < NumShaderTypes; type++)
	{
		glAttachShader(program, shaders[type]);
	}
	glLinkProgram(program);
	for(unsigned int type = 0; type < NumShaderTypes; type++)
	{
		glDetachShader(program, shaders[type]);
		glDeleteShader(shaders[type]);
	}
	GLint good;
	glGetProgramiv(program, GL_LINK_STATUS, &good);
	if(good == GL_FALSE)
	{
		GLint logLength;
		std::string log;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		log.resize(logLength);
		glGetProgramInfoLog(program, logLength, 0, &log[0]);
		glDeleteProgram(program);
		throw std::runtime_error("Error linking shader: " + log);
	}
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::activate()
{
	glUseProgram(program);
}

void Shader::deactivate()
{
	glUseProgram(0);
}

