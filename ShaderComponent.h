#pragma once

#include <string>
#include <vector>

// Shader Component
/*
Each file is formated like this:
[vertex|fragment]
[input variables]
[output variables]
[code]
**/

class ShaderComponent
{
public:
	enum
	{
		Vertex,
//		Geometry, (later)
		Fragment,
		NumTypes
	};

	ShaderComponent(std::string const & filename);

	std::string const & getFilename() const;

	int getType() const;
	
	std::vector<std::string> const & getInputs() const;
	
	std::vector<std::string> const & getOutputs() const;
	
	std::string const & getCode() const;

private:
	std::string mFilename;
	int mType;
	std::vector<std::string> mInputs;
	std::vector<std::string> mOutputs;
	std::string mCode;
};

