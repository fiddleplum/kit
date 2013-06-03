#pragma once

#include <vector>

class Model;

class ModelGL
{
public:
	ModelGL(Model const & model);

	void render() const;

private:
	std::vector<unsigned int> vertexBufferObjects;
};

