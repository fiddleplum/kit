#include "Shader.h"
#include "OpenGL.h"
#include "../StringUtil.h"
#include <vector>
#include <stdexcept>

Shader::Shader(std::string code [NumTypes])
{
	std::vector<unsigned int> shaderObjects;
	try
	{
		for(unsigned int type = 0; type < NumTypes; type++)
		{
			if(!code[type].empty())
			{
				shaderObjects.push_back(compileShaderObject((Type)type, code[type]));
			}
		}
	}
	catch(...)
	{
		for(unsigned int shaderObject : shaderObjects)
		{
			glDeleteShader(shaderObject);
		}
		throw;
	}
	mProgram = linkShaderProgram(shaderObjects); // delete shader objects as well
	populateVariableLocations();
}

Shader::~Shader()
{
	glDeleteProgram(mProgram);
}

int Shader::getUniformLocation(std::string const & name) const
{
	auto it = mUniforms.find(name);
	if(it == mUniforms.end())
	{
		return -1;
	}
	return it->second;
}

int Shader::getAttributeLocation(std::string const & name) const
{
	auto it = mAttributes.find(name);
	if(it == mAttributes.end())
	{
		return -1;
	}
	return it->second;
}

void Shader::activate()
{
	glUseProgram(mProgram);
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

unsigned int Shader::compileShaderObject(Type type, std::string const & code)
{
	unsigned int glType = 0;
	if(type == Vertex)
	{
		glType = GL_VERTEX_SHADER;
	}
	else if(type == Fragment)
	{
		glType = GL_FRAGMENT_SHADER;
	}
	else
	{
		throw std::runtime_error("Unknown shader object type '" + std::to_string(type) + "', with code:\n" + code + "\n");
	}
	unsigned int handle;
	handle = glCreateShader(glType);
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
		throw std::runtime_error("Error compiling shader object " + std::to_string(type) + ".\n\nLog:\n" + log + "\n\nCode:\n" + code + "\n");
	}
}

unsigned int Shader::linkShaderProgram(std::vector<unsigned int> const & shaderObjects)
{
	unsigned int program = glCreateProgram();
	for(unsigned int shaderObject : shaderObjects)
	{
		glAttachShader(program, shaderObject);
	}
	glLinkProgram(program);
	for(unsigned int shaderObject : shaderObjects)
	{
		glDetachShader(program, shaderObject);
		glDeleteShader(shaderObject);
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
	return program;
}

void Shader::populateVariableLocations()
{
	GLint numVariables;
	GLint maxNameSize;
	std::string name;
	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &numVariables);
	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
	for(int i = 0; i < numVariables; i++)
	{
		GLsizei nameSize;
		GLint size;
		GLenum type;
		name.resize(maxNameSize);
		glGetActiveUniform(mProgram, i, maxNameSize, &nameSize, &size, &type, &name[0]);
		name.resize(nameSize);
		GLint location = glGetUniformLocation(mProgram, name.c_str());
		if(location != -1)
		{
			mUniforms[name] = location;
		}
	}
	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &numVariables);
	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameSize);
	for(int i = 0; i < numVariables; i++)
	{
		GLsizei nameSize;
		GLint size;
		GLenum type;
		name.resize(maxNameSize);
		glGetActiveAttrib(mProgram, i, maxNameSize, &nameSize, &size, &type, &name[0]);
		name.resize(nameSize);
		GLint location = glGetAttribLocation(mProgram, name.c_str());
		if(location != -1)
		{
			mAttributes[name] = location;
		}
	}
}

