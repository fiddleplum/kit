#include "font.h"
#include "math_util.h"
#include "texture.h"
#include "text.h"
#include "gui_model.h"
#include "scene_model.h"
#include "math_util.h"
#include <SDL_ttf.h>

unsigned int numFontsLoaded = 0;
unsigned int numCharsInRow = 16;
unsigned int numCharsInCol = 8;
unsigned int numCharsInBlock = numCharsInRow * numCharsInCol;

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

void Font::getGuiModelsFromText(std::string const & text, std::vector<OwnPtr<GuiModel>> & models, Coord2i & textSize)
{
	models.clear();
	textSize = {0, 0};
	std::map<Ptr<Texture>, int> texturesToModels;
	std::vector<std::vector<GuiModel::Vertex>> vertices;
	std::vector<std::vector<unsigned int>> indices;
	Text unicodeText = text;
	Coord2i pos;
	for(auto c : unicodeText)
	{
		Recti bounds;
		Recti uvBounds;
		Ptr<Texture> texture;
		bool hasInfo = getInfoFromChar(c, pos, bounds, uvBounds, texture);
		if(!hasInfo)
		{
			continue;
		}
		// Get the model (or setup a new one)
		int modelIndex = 0;
		auto texturesToModelsIt = texturesToModels.find(texture);
		if(texturesToModelsIt == texturesToModels.end()) // Create a new model for new texture.
		{
			OwnPtr<GuiModel> model;
			model.setNew();
			model->setTexture(texture);
			models.push_back(model);
			vertices.push_back(std::vector<GuiModel::Vertex>());
			indices.push_back(std::vector<unsigned int>());
			modelIndex = models.size() - 1;
			texturesToModels[texture] = modelIndex;
		}
		// Do the vertices
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
		textSize[0] = Math::max(textSize[0], bounds.max[0]);
		textSize[1] = Math::max(textSize[1], bounds.max[1]);
	}
	for(unsigned int i = 0; i < models.size(); i++)
	{
		models[i]->setVertices(vertices[i]);
		models[i]->setIndices(indices[i]);
	}
}

void Font::getSceneModelsFromText(std::string const & text, std::vector<OwnPtr<SceneModel>> & models, Coord2i & textSize)
{
	models.clear();
	textSize = {0, 0};
	std::map<Ptr<Texture>, int> texturesToModels;
	std::vector<std::vector<Coord4i>> vertices;
	std::vector<std::vector<unsigned int>> indices;
	Text unicodeText = text;
	Coord2i pos;
	for(auto c : unicodeText)
	{
		Recti bounds;
		Recti uvBounds;
		Ptr<Texture> texture;
		bool hasInfo = getInfoFromChar(c, pos, bounds, uvBounds, texture);
		if(!hasInfo)
		{
			continue;
		}
		// Get the model (or setup a new one)
		int modelIndex = 0;
		auto texturesToModelsIt = texturesToModels.find(texture);
		if(texturesToModelsIt == texturesToModels.end()) // Create a new model for new texture.
		{
			OwnPtr<SceneModel> model;
			model.setNew();
			model->setVertexFormat(false, false, false, 1);
			model->addTexture(texture, "diffuse", 0);
			models.push_back(model);
			vertices.push_back(std::vector<Coord4i>());
			indices.push_back(std::vector<unsigned int>());
			modelIndex = models.size() - 1;
			texturesToModels[texture] = modelIndex;
		}
		// Do the vertices
		indices[modelIndex].push_back(vertices[modelIndex].size() + 0);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 3);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 2);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 2);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 1);
		indices[modelIndex].push_back(vertices[modelIndex].size() + 0);
		Coord4i vertex;
		vertices[modelIndex].push_back({bounds.min[0], bounds.min[1], uvBounds.min[0], uvBounds.min[1]});
		vertices[modelIndex].push_back({bounds.max[0], bounds.min[1], uvBounds.max[0], uvBounds.min[1]});
		vertices[modelIndex].push_back({bounds.max[0], bounds.max[1], uvBounds.max[0], uvBounds.max[1]});
		vertices[modelIndex].push_back({bounds.min[0], bounds.max[1], uvBounds.min[0], uvBounds.max[1]});
		textSize[0] = Math::max(textSize[0], bounds.max[0]);
		textSize[1] = Math::max(textSize[1], bounds.max[1]);
	}
	for(unsigned int i = 0; i < models.size(); i++)
	{
		models[i]->setVertices(&vertices[i][0], sizeof(Coord4i) * vertices[i].size());
		models[i]->setIndices(&indices[i][0], indices[i].size());
	}
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

bool Font::getInfoFromChar(unsigned int c, Coord2i & pos, Recti & bounds, Recti & uvBounds, Ptr<Texture> & texture)
{
	std::map<Ptr<Texture>, int> texturesToModels;
	std::vector<OwnPtr<GuiModel>> models;
	std::vector<std::vector<GuiModel::Vertex>> vertices;
	std::vector<std::vector<unsigned int>> indices;
	if(c == '\n')
	{
		pos[1] += heightOfChar;
		pos[0] = 0;
		return false;
	}
	if(c == '\t')
	{
		pos[0] += heightOfChar;
		return false;
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
	// Set the texture.
	texture = block.texture;
	// Set the bounds and uvBounds.
	bounds.min = {pos[0] - (int)(block.cellSize - block.widths[c - blockStart]) / 2, pos[1] - (int)(block.cellSize - heightOfChar) / 2};
	bounds.max = bounds.min + Coord2i{(int)block.cellSize, (int)block.cellSize};
	uvBounds.min = {(int)((c - blockStart) % numCharsInRow * block.cellSize), (int)((c - blockStart) / numCharsInRow * block.cellSize)};
	uvBounds.max = uvBounds.min + Coord2i{(int)block.cellSize, (int)block.cellSize};
	pos[0] += block.widths[c - blockStart];
	return true;
}

