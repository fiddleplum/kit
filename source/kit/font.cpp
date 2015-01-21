#include "font.h"
#include "math_util.h"
#include "texture.h"
#include "text.h"
#include <SDL_ttf.h>

int numFontsLoaded = 0;
int numCharsInBlock = 128;
int numCharsInRow = 16;

Font::Font(std::string const & filename, int size_)
{
	size = size_;
	if(numFontsLoaded == 0)
	{
		int status = 0;
		status = TTF_Init();
		if(status == -1)
		{
			throw std::runtime_error(std::string() + "SDL_ttf failed to initialize. " + TTF_GetError());
		}
	}
	// Load the font.
	ttfFont = TTF_OpenFont(filename.c_str(), size_);
	if(ttfFont == 0)
	{
		throw std::runtime_error("The font '" + filename + "' at size " + std::to_string(size_) + " could not be loaded. ");
	}
	numFontsLoaded++;
	loadBlock(0);
}

Font::~Font()
{
	TTF_CloseFont(ttfFont);
	numFontsLoaded--;
	if(numFontsLoaded == 0)
	{
		TTF_Quit();
	}
}

std::vector<OwnPtr<GuiModel>> Font::getGuiModelsFromText(std::string const & text)
{
	std::vector<OwnPtr<GuiModel>> models;
	Text unicodeText = text;
	for(auto c : unicodeText)
	{
		int blockStart = c / numCharsInBlock * numCharsInBlock;
		auto blockIt = blocks.find(blockStart);
		if(blockIt == blocks.end())
		{
			loadBlock(blockStart);
			blockIt = blocks.find(blockStart);
		}

	}
	return models;
}

void Font::loadBlock(int blockStart)
{
	// Create a texture for 128 characters, starting with the unicodeStart.
	Block block;
	int numCharsInCol = numCharsInBlock / numCharsInRow;
	block.widths.resize(numCharsInBlock);
	int sizePow2 = Math::ceilPow2(size);
	SDL_Color white = {255, 255, 255, 255};
	SDL_Surface * surface = SDL_CreateRGBSurface(0, sizePow2 * numCharsInRow, sizePow2 * numCharsInCol, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	for(int i = 0; i < numCharsInBlock; ++i)
	{
		SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended(ttfFont, blockStart + i, white);
		if(glyphSurface != 0)
		{
			heightOfChar = glyphSurface->h;
			block.widths[i] = glyphSurface->w;
			SDL_Rect rect;
			rect.x = (i % numCharsInRow) * sizePow2 + (sizePow2 - glyphSurface->w) / 2;
			rect.w = glyphSurface->w;
			rect.y = (i / numCharsInRow) * sizePow2 + (sizePow2 - glyphSurface->h) / 2;
			rect.h = glyphSurface->h;
			SDL_BlitSurface(glyphSurface, 0, surface, &rect);
			SDL_FreeSurface(glyphSurface);
		}
	}
	SDL_SaveBMP(surface, "test.bmp");
	block.texture.setNew(surface->pixels, Coord2i{surface->w, surface->h});
	SDL_FreeSurface(surface);
	blocks[blockStart] = block;
}

