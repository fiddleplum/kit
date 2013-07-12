#include "Shader.h"
#include "OpenGL.h"
#include <stdexcept>

Shader::Shader(std::string const & vertexCode, std::string const & fragmentCode)
{
	unsigned int vertexShaderObject = 0;
	unsigned int fragmentShaderObject = 0;
	try
	{
		vertexShaderObject = compileShaderObject(vertexCode, GL_VERTEX_SHADER);
		fragmentShaderObject = compileShaderObject(fragmentCode, GL_FRAGMENT_SHADER);
	}
	catch(...)
	{
		glDeleteShader(vertexShaderObject);
		glDeleteShader(fragmentShaderObject);
		throw;
	}
	linkShaderProgram(vertexShaderObject, fragmentShaderObject);
	populateVariableLocations();
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

int Shader::getUniformLocation(std::string const & name) const
{
	auto it = uniforms.find(name);
	if(it == uniforms.end())
	{
		return -1;
	}
	return it->second;
}

int Shader::getAttributeLocation(std::string const & name) const
{
	auto it = attributes.find(name);
	if(it == attributes.end())
	{
		return -1;
	}
	return it->second;
}

void Shader::activate()
{
	glUseProgram(program);
}

void Shader::deactivate()
{
	glUseProgram(0);
}

void Shader::setUniform(int location, int value)
{
	glUniform1i(location, value);
}

void Shader::setUniform(int location, float value)
{
	glUniform1f(location, value);
}

void Shader::setUniform(int location, Vector2i value)
{
	glUniform2iv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector2f value)
{
	glUniform2fv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector3i value)
{
	glUniform3iv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector3f value)
{
	glUniform3fv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector4i value)
{
	glUniform4iv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector4f value)
{
	glUniform4fv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Matrix44f value)
{
	glUniformMatrix4fv(location, 1, false, value.ptr());
}

unsigned int Shader::compileShaderObject(std::string const & code, unsigned int type)
{
	unsigned int handle;
	handle = glCreateShader(type);
	char const * shaderCode = code.c_str();
	GLint shaderCodeSize = code.size();
	glShaderSource(handle, 1, &shaderCode, &shaderCodeSize);
	glCompileShader(handle);
	GLint good;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &good);
	if(good == GL_FALSE)
	{
		GLint logLength;
		std::string log;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		log.resize(logLength);
		glGetShaderInfoLog(handle, logLength, 0, &log[0]);
		std::string typeString;
		glDeleteShader(handle);
		throw std::runtime_error("Error compiling shader object: " + log);
	}
}

void Shader::linkShaderProgram(unsigned int vertexShaderObject, unsigned int fragmentShaderObject)
{
	program = glCreateProgram();
	glAttachShader(program, vertexShaderObject);
	glAttachShader(program, fragmentShaderObject);
	glLinkProgram(program);
	glDetachShader(program, fragmentShaderObject);
	glDetachShader(program, vertexShaderObject);
	glDeleteShader(fragmentShaderObject);
	glDeleteShader(vertexShaderObject);
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

void Shader::populateVariableLocations()
{
	GLint numVariables;
	GLint maxNameSize;
	std::string name;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numVariables);
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
	for(int i = 0; i < numVariables; i++)
	{
		GLsizei nameSize;
		GLint size;
		GLenum type;
		name.resize(maxNameSize);
		glGetActiveUniform(program, i, maxNameSize, &nameSize, &size, &type, &name[0]);
		name.resize(nameSize);
		GLint location = glGetUniformLocation(program, name.c_str());
		if(location != -1)
		{
			uniforms[name] = location;
		}
	}
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numVariables);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameSize);
	for(int i = 0; i < numVariables; i++)
	{
		GLsizei nameSize;
		GLint size;
		GLenum type;
		name.resize(maxNameSize);
		glGetActiveAttrib(program, i, maxNameSize, &nameSize, &size, &type, &name[0]);
		name.resize(nameSize);
		GLint location = glGetAttribLocation(program, name.c_str());
		if(location != -1)
		{
			attributes[name] = location;
		}
	}
}

