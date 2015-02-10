#pragma once

#include "coord.h"
#include "ptr.h"
#include "rect.h"
#include <string>
#include <map>
#include <vector>

#pragma comment (lib, "SDL2_ttf.lib")

typedef struct _TTF_Font TTF_Font;
class GuiModel;
class SceneModel;
class Texture;

class Font
{
public:
	Font(std::string const & filename, int size);
	~Font();
	void getGuiModelsFromText(std::string const & text, std::vector<OwnPtr<GuiModel>> & models, Coord2i & textSize);
	void getSceneModelsFromText(std::string const & text, std::vector<OwnPtr<SceneModel>> & models, Coord2i & textSize);

private:
	void loadBlock(int unicodeStart);
	bool getInfoFromChar(unsigned int c, Coord2i & pos, Recti & bounds, Recti & uvBounds, Ptr<Texture> & texture);

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

