#include "Gui.h"
#include "Widget.h"
#include "OpenGL.h"

Gui::Gui()
{
	widget = nullptr;
}

void Gui::setWindowSize(Vector2i newWindowSize)
{
	windowSize = newWindowSize;
}

void Gui::setWidget(Widget * newWidget)
{
	widget = newWidget;
}

void Gui::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, windowSize[0], windowSize[1]);
	glViewport(0, 0, windowSize[0], windowSize[1]);

	if(widget != nullptr)
	{
		widget->render();
	}
}

