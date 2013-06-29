#include "Shader.h"

#include "OpenGL.h"
#include <cassert>
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

Shader::Shader(Config const & config)
{
	unsigned int vertexShaderObject = 0;
	unsigned int fragmentShaderObject = 0;
	try
	{
		vertexShaderObject = createVertexShaderObject(config);
		fragmentShaderObject = createFragmentShaderObject(config);
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

// Shader::Shader(std::vector<ShaderComponent> const & shaderComponents)
// {
	// std::string code [ShaderComponent::NumTypes];
	// std::set<std::string> uniforms [ShaderComponent::NumTypes];
	// std::set<std::string> attributes [ShaderComponent::NumTypes];

	// // go through inputs and outputs, sorting and checking them
	// for(int sci = 0; sci < shaderComponents.size(); sci++)
	// {
		// ShaderComponent const & sc = shaderComponents[sci];
		// for(int i = 0; i < sc.getInputs().size(); i++)
		// {
			// std::string const & input = sc.getInputs()[i];
			// bool findValidOutput = false;
			// if(input.beginsWith("attribute "))
			// {
				// if(sc.getType() != ShaderComponent::Vertex)
				// {
					// throw std::runtime_error("In shader component " + sc.getName() + ": Only vertex shader components may have attribute inputs.");
				// }
				// findValidOutput = true;
			// }
			// if(findValidOutput)
			// {
				// for(int scj = 0; scj < i; scj++) // check all previous shader components for an output
				// {
				// }
			// }
		// }
	// }

	// for(int type = 0; type < ShaderComponent::NumTypes; type++)
	// {
	// }
// }

// Shader::Shader(std::vector<std::string> const & code)
// {
	// assert(code.size() == 3);

	// bool shaderValid [NumShaderTypes];
	// for(unsigned int type = 0; type < NumShaderTypes; type++)
	// {
		// shaderValid[type] = false;
	// }

	// GLuint shaders[NumShaderTypes];
	// for(unsigned int type = 0; type < NumShaderTypes; type++)
	// {
		// // Does this type have any code with it?
		// if(code.empty())
		// {
			// continue;
		// }

		// // Set the OpenGL type.
		// GLenum glType;
		// switch(type)
		// {
		// case Vertex:
			// glType = GL_VERTEX_SHADER; break;
		// case Geometry:
			// glType = GL_GEOMETRY_SHADER; break;
		// case Fragment:
			// glType = GL_FRAGMENT_SHADER; break;
		// }

		// // Create the shader objects.
		// shaders[type] = glCreateShader(type);
		// char const * shaderCode = code[type].c_str();
		// GLint shaderCodeSize = code[type].size();
		// glShaderSource(shaders[type], 1, &shaderCode, &shaderCodeSize);
		// glCompileShader(shaders[type]);
		// GLint good;
		// glGetShaderiv(shaders[type], GL_COMPILE_STATUS, &good);
		// if(good == GL_FALSE)
		// {
			// GLint logLength;
			// std::string log;
			// glGetShaderiv(shaders[type], GL_INFO_LOG_LENGTH, &logLength);
			// log.resize(logLength);
			// glGetShaderInfoLog(shaders[type], logLength, 0, &log[0]);
			// std::string typeString;
			// switch(type)
			// {
			// case Vertex:
				// typeString = "vertex"; break;
			// case Geometry:
				// typeString = "geometry"; break;
			// case Fragment:
				// typeString = "fragment"; break;
			// }
			// for(unsigned int typeToDelete = 0; typeToDelete <= type; typeToDelete++)
			// {
				// glDeleteShader(shaders[typeToDelete]);
			// }
			// throw std::runtime_error("Error compiling " + typeString + " shader: " + log);
		// }
		// shaderValid[type] = true;
	// }

	// // Create the shader program.
	// program = glCreateProgram();
	// for(unsigned int type = 0; type < NumShaderTypes; type++)
	// {
		// glAttachShader(program, shaders[type]);
	// }
	// glLinkProgram(program);
	// for(unsigned int type = 0; type < NumShaderTypes; type++)
	// {
		// glDetachShader(program, shaders[type]);
		// glDeleteShader(shaders[type]);
	// }
	// GLint good;
	// glGetProgramiv(program, GL_LINK_STATUS, &good);
	// if(good == GL_FALSE)
	// {
		// GLint logLength;
		// std::string log;
		// glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		// log.resize(logLength);
		// glGetProgramInfoLog(program, logLength, 0, &log[0]);
		// glDeleteProgram(program);
		// throw std::runtime_error("Error linking shader: " + log);
	// }
	
	// populateVariableLocations();
// }

Shader::~Shader()
{
	glDeleteProgram(program);
}

int Shader::getUniformLocation(std::string const & name)
{
	auto it = uniforms.find(name);
	if(it == uniforms.end())
	{
		return -1;
	}
	return it->second;
}

int Shader::getAttributeLocation(std::string const & name)
{
	auto it = attributes.find(name);
	if(it == attributes.end())
	{
		return -1;
	}
	return it->second;
}

void Shader::setUniform(int location, int value) const
{
	glUniform1i(location, value);
}

void Shader::setUniform(int location, float value) const
{
	glUniform1f(location, value);
}

void Shader::setUniform(int location, Vector2i value) const
{
	glUniform2iv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector2f value) const
{
	glUniform2fv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector3i value) const
{
	glUniform3iv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector3f value) const
{
	glUniform3fv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector4i value) const
{
	glUniform4iv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Vector4f value) const
{
	glUniform4fv(location, 1, value.ptr());
}

void Shader::setUniform(int location, Matrix44f value) const
{
	glUniformMatrix4fv(location, 1, false, value.ptr());
}

void Shader::activate()
{
	glUseProgram(program);
}

void Shader::deactivate()
{
	glUseProgram(0);
}

unsigned int Shader::createVertexShaderObject(Config const & config)
{
	std::string code;

	code += "#version 120\n";

	// Add the global variables.
	code += "attribute vec3 aPosition;\n";
	if(config.usesWorldView)
	{
		code += "uniform mat4 uWorldView;\n";
	}
	code += "uniform mat4 uProjection;\n";

	// Add the main function.
	code += "void main()\n";
	code += "{\n";
	code += "	vec3 position = aPosition\n";
	if(config.usesWorldView)
	{
		code += "	position = uWorldView * position;\n";
	}
	code += "	gl_Position = uProjection * position;\n";
	code += "}\n";
	
	return compileShaderObject(code, GL_VERTEX_SHADER);
}

unsigned int Shader::createFragmentShaderObject(Config const & config)
{
	std::string code;
	
	code += "#version 120\n";
	
	code += "void main()\n";
	code += "{\n";
	code += "	gl_Color = vec4(1,1,1,1);\n";
	code += "}\n";
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

