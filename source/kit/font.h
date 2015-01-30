#pragma once

#include "coord.h"
#include "ptr.h"
#include <string>
#include <map>
#include <vector>

#pragma comment (lib, "SDL2_ttf.lib")

typedef struct _TTF_Font TTF_Font;
class GuiModel;
class Texture;

class Font
{
public:
	Font(std::string const & filename, int size);
	~Font();
	std::vector<OwnPtr<GuiModel>> getGuiModelsFromText(std::string const & text, Coord2i & textSize);

private:
	void loadBlock(int unicodeStart);

	struct Block
	{
		OwnPtr<Texture> texture;
		std::vector<unsigned int> widths;
		unsigned int cellSize;
	};

	std::map<int, Block> blocks;
	TTF_Font * ttfFont;
	int size;
	unsigned int heightOfChar;
};

