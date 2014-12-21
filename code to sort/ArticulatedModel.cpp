#include "ArticulatedModel.h"
#include "Serialize.h"
#include <fstream>
#include <stdexcept>

ArticulatedModel::ArticulatedModel(std::string const & filename)
{
	std::fstream file(filename, std::ios::binary | std::ios::in);
	try
	{
		int numMeshes = 0;
		deserialize(file, numMeshes);
		for(int modelI = 0; modelI < numMeshes; modelI++)
		{
			models.push_back(Model(file));
		}
	}
	catch (std::exception const &)
	{
		throw std::runtime_error("Error while loading " + filename + ". ");
	}
}

