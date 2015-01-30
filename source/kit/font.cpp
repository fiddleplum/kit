#include "font.h"
#include "math_util.h"
#include "texture.h"
#include "text.h"
#include "gui_model.h"
#include "rect.h"
#include "math_util.h"
#include <SDL_ttf.h>

unsigned int numFontsLoaded = 0;
unsigned int numCharsInRow = 16;
unsigned int numCharsInCol = 8;

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

std::vector<OwnPtr<GuiModel>> Font::getGuiModelsFromText(std::string const & text, Coord2i & textSize)
{
	textSize = {0, 0};
	unsigned int numCharsInBlock = numCharsInRow * numCharsInCol;
	std::map<Ptr<Texture>, int> texturesToModels;
	std::vector<OwnPtr<GuiModel>> models;
	std::vector<std::vector<GuiModel::Vertex>> vertices;
	std::vector<std::vector<unsigned int>> indices;
	Text unicodeText = text;
	Coord2i pos;
	for(auto c : unicodeText)
	{
		if(c == '\n')
		{
			pos[1] += heightOfChar;
			pos[0] = 0;
			continue;
		}
		if(c == '\t')
		{
			pos[0] += heightOfChar;
			continue;
		}
		// Get the block (or load a new one)
		auto blockStart = c / numCharsInBlock * numCharsInBlock;
		auto blockIt = blocks.find(blockStart);
		if(blockIt == blocks.end())
		{
			loadBlock(blockStart);
			blockIt = blocks.find(blockStart);
		}
		auto block = blockIt->second;
		// Get the model (or setup a new one)
		int modelIndex = 0;
		auto texturesToModelsIt = texturesToModels.find(blockIt->second.texture);
		if(texturesToModelsIt == texturesToModels.end()) // Create a new model for new texture.
		{
			OwnPtr<GuiModel> model;
			model.setNew();
			model->setTexture(block.texture);
			models.push_back(model);
			vertices.push_back(std::vector<GuiModel::Vertex>());
			indices.push_back(std::vector<unsigned int>());
			modelIndex = models.size() - 1;
			texturesToModels[blockIt->second.texture] = modelIndex;
		}
		// Do the vertices
		Recti bounds;
		Recti uvBounds;
		bounds.min = {pos[0] - (int)(block.cellSize - block.widths[c - blockStart]) / 2, pos[1] - (int)(block.cellSize - heightOfChar) / 2};
		bounds.max = bounds.min + Coord2i{(int)block.cellSize, (int)block.cellSize};
		uvBounds.min = {(int)((c - blockStart) % numCharsInRow * block.cellSize), (int)((c - blockStart) / numCharsInRow * block.cellSize)};
		uvBounds.max = uvBounds.min + Coord2i{(int)block.cellSize, (int)block.cellSize};
		indices[modelIndex].push_back(vertices[modelIndex].size() + 0);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 3);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 2);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 2);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 1);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 0);
		GuiModel::Vertex vertex;
		vertex.pos = {bounds.min[0], bounds.min[1]};
		vertex.uv = {uvBounds.min[0], uvBounds.min[1]};
		vertices[modelIndex].push_back(vertex);
		vertex.pos = {bounds.max[0], bounds.min[1]};
		vertex.uv = {uvBounds.max[0], uvBounds.min[1]};
		vertices[modelIndex].push_back(vertex);
		vertex.pos = {bounds.max[0], bounds.max[1]};
		vertex.uv = {uvBounds.max[0], uvBounds.max[1]};
		vertices[modelIndex].push_back(vertex);
		vertex.pos = {bounds.min[0], bounds.max[1]};
		vertex.uv = {uvBounds.min[0], uvBounds.max[1]};
		vertices[modelIndex].push_back(vertex);
		pos[0] += block.widths[c - blockStart];
		textSize[0] = Math::max(textSize[0], bounds.max[0]);
		textSize[1] = Math::max(textSize[1], bounds.max[1]);
	}
	for(unsigned int i = 0; i < models.size(); i++)
	{
		models[i]->setVertices(vertices[i]);
		models[i]->setIndices(indices[i]);
	}
	return models;
}

void Font::loadBlock(int blockStart)
{
	// Create a texture for 128 characters, starting with the unicodeStart.
	Block block;
	int numCharsInBlock = numCharsInRow * numCharsInCol;
	block.widths.resize(numCharsInBlock);
	block.cellSize = 2 * Math::ceilPow2(size);
	SDL_Color white = {255, 255, 255, 255};
	SDL_Surface * surface = SDL_CreateRGBSurface(0, block.cellSize * numCharsInRow, block.cellSize * numCharsInCol, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	for(int i = 0; i < numCharsInBlock; ++i)
	{
		SDL_Surface * glyphSurface = TTF_RenderGlyph_Solid(ttfFont, blockStart + i, white);
		if(glyphSurface != 0)
		{
			heightOfChar = glyphSurface->h;
			block.widths[i] = glyphSurface->w;
			SDL_Rect rect;
			rect.x = (i % numCharsInRow) * block.cellSize + (block.cellSize - glyphSurface->w) / 2;
			rect.y = (i / numCharsInRow) * block.cellSize + (block.cellSize - glyphSurface->h) / 2;
			SDL_BlitSurface(glyphSurface, 0, surface, &rect);
			SDL_FreeSurface(glyphSurface);
		}
	}
	SDL_SaveBMP(surface, "test.bmp");
	block.texture.setNew(surface->pixels, Coord2i{surface->w, surface->h});
	SDL_FreeSurface(surface);
	blocks[blockStart] = block;
}

