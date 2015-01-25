#pragma once

#include "gui_element.h"
#include "ptr.h"
#include <vector>
#include <string>

class Font;
class GuiModel;

class GuiText : public GuiElement
{
public:
	GuiText();

	Recti getBounds() const override;

	void setPosition(Coord2i position) override;

	void setSize(Coord2i size) override;

	void setFont(std::string const & filename, int size);

	void setText(std::string const & text);

	bool handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid) override;

	void render(Coord2i windowSize) const override;

private:
	std::vector<OwnPtr<GuiModel>> models;
	Recti bounds;
	Ptr<Font> font;
};

