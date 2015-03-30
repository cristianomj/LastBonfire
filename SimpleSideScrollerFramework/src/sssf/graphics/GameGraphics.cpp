/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGraphics.cpp

	See GameGraphics.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\game\IllegalArgumentException.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\text\TextFileWriter.h"

/*
	GameGraphics - Default constructor, nothing to initialize.
*/
GameGraphics::GameGraphics()
{
	debugTextShouldBeRendered = false;
}

/*
	~GameGraphics - Destructor, it cleans up the render lists and texture
	managers. This should only be called when the application is closing.
*/
GameGraphics::~GameGraphics()
{
	delete guiRenderList;
	delete guiTextureManager;
	delete worldRenderList;
	delete worldTextureManager;
}

/*
	clearWorldTextures - When the game leaves a level we have to clear
	out these data structures. Calling clear on these will delete
	all the objects inside.
*/
void GameGraphics::clearWorldTextures()
{	
	// CLEAR LEVEL DATA STRUCURES
	worldTextureManager->clear();
	worldRenderList->clear();
}

/*
	fillRenderLists - This method causes the render lists to be 
	filled with the things that have to be drawn this frame.
*/
void GameGraphics::fillRenderLists(Game *game)
{
	// GENERATE RENDER LISTS FOR GAME WORLD AND GUI
	GameStateManager *gsm = game->getGSM();
	gsm->addGameRenderItemsToRenderList(game);
	GameGUI *gui = game->getGUI();
	gui->addRenderItemsToRenderList(game);
}

/*
	init - This method constructs the data structures for managing textures
	and render lists. It calls the createTextureManager, which is technology
	specific, and so is implemented only by child classes.
*/
void GameGraphics::init(int initScreenWidth, int initScreenHeight)
{
	// INIT SCREEN DIMENSIONS
	screenWidth = initScreenWidth;
	screenHeight = initScreenHeight;

	// GUI TEXTURES (like buttons, cursor, etc.)
	guiRenderList = new RenderList(DEFAULT_INIT_GUI_RENDER_LIST_SIZE);
	guiTextureManager = createTextureManager();

	// LEVEL TEXTURES (like sprites, tiles, particles, etc.)
	worldRenderList = new RenderList(DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE);
	worldTextureManager = createTextureManager();
}

/*
	renderText - This method will go through the GameText argument,
	pull out each TextToDraw object, and use a technology-specific
	method in a child class, renderTextToDraw, to render each
	piece of text.
*/
void GameGraphics::renderText(GameText *text)
{
	// FOR NOW WE ONLY USE TEXT RENDERING FOR DEBUG TEXT, BUT
	// IT COULD ALSO BE USED FOR GAME TEXT LIKE HEALTH STATS
	if (debugTextShouldBeRendered)
	{
		int numTextObjects = text->getNumTextObjectsToDraw();
		for (int i = 0; i < numTextObjects; i++)
		{
			TextToDraw *textToDraw = text->getTextToDrawAtIndex(i);
			renderTextToDraw(textToDraw);
		}
	}
}