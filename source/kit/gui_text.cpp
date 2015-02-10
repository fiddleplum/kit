#include "gui_text.h"
#include "gui_model.h"
#include "resources.h"
#include "font.h"

GuiText::GuiText()
{
}

Recti GuiText::getBounds() const
{
	return bounds;
}

void GuiText::setPosition(Coord2i position_)
{
	bounds.max += position_ - bounds.min;
	bounds.min = position_;
	for(auto model : models)
	{
		model->setPosition(position_);
	}
}

void GuiText::setSize(Coord2i size)
{
	// does nothing
}

void GuiText::setFont(std::string const & filename, int size)
{
	font = fontCache->load(filename + std::to_string(size), filename, size);
}

void GuiText::setText(std::string const & text)
{
	if(!font.isValid())
	{
		throw std::runtime_error("Please set a font first. ");
	}
	Coord2i textSize;
	font->getGuiModelsFromText(text, models, textSize);
	bounds.max = bounds.min + textSize;
	for(auto model : models)
	{
		model->setPosition(bounds.min);
	}
}

bool GuiText::handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)
{
	if(event.is(Event::MouseButton))
	{
		auto mbEvent = event.as<MouseButtonEvent>();
		if(mbEvent.pressed && cursorIsOver(cursorPosition, cursorPositionIsValid))
		{
			return true;
		}
	}
	else if(event.is(Event::MouseWheel))
	{
		if(cursorIsOver(cursorPosition, cursorPositionIsValid))
		{
			return true;
		}
	}
	return false;
}

void GuiText::render(Coord2i windowSize) const
{
	for(auto model : models)
	{
		model->render(windowSize);
	}
}

