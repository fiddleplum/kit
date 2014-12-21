#pragma once

#include "Model.h"
#include <map>
#include <string>

class ArticulatedModel
{
public:
	ArticulatedModel(std::string const & filename);

	std::vector<Model> models;
};

/*

ArticulatedModel format:

Model list - list of models.

*/