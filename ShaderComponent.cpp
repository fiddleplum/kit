#include "ShaderComponent.h"
#include "StringUtil.h"
#include <fstream>
#include <stdexcept>
#include <cassert>

ShaderComponent::ShaderComponent(std::string const & filename)
{
	mFilename = filename;

	std::fstream file {filename, std::fstream::in};
	
	// get type
	std::string type;
	std::getline(file, type);
	type = trim(type);
	if(type == "vertex")
	{
		mType = Vertex;
	}
	else if(type == "fragment")
	{
		mType = Fragment;
	}
	else
	{
		throw std::runtime_error {"While loading '" + filename + "': unkown type."};
	}

	// get inputs
	std::string inputs;
	std::getline(file, inputs);
	mInputs = split(inputs, ',');
	
	// get outputs
	std::string outputs;
	std::getline(file, outputs);
	mOutputs = split(outputs, ',');

	// get code
	do
	{
		char c = (char)file.get();
		if(file.good())
		{
			mCode += (char)c;
		}
		else
		{
			break;
		}
	} while(true);
}

std::string const & ShaderComponent::getFilename() const
{
	return mFilename;
}

int ShaderComponent::getType() const
{
	return mType;
}

std::vector<std::string> const & ShaderComponent::getInputs() const
{
	return mInputs;
}

std::vector<std::string> const & ShaderComponent::getOutputs() const
{
	return mOutputs;
}

std::string const & ShaderComponent::getCode() const
{
	return mCode;
}

