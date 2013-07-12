#include "RenderEngine.h"

bool RenderEngine::ModelLess::operator <(Model const * model0, Model const * model1);
{
	if(model0->getShader() < model1->getShader())
	{
		return true;
	}
	else if(model0->getShader() > model1->getShader())
	{
		return false;
	}
	for(unsigned int i = 0; i < std::min(model0->getTextures()
	if(model0->
}
