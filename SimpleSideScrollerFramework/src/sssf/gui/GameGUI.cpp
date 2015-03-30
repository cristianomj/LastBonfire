/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGUI.cpp

	See GameGUI.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\game\IllegalArgumentException.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Button.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\input\ButtonEventHandler.h"
#include "sssf\input\GameInput.h"
#include "sssf\text\BufferedTextFileReader.h"

/*
	GameGUI - Default Constructor, it sets up the data structure for storing
	all ScreenGUIs. These should subsequently be added using addScreenGUI
*/
GameGUI::GameGUI()
{
	customCursor = NULL;
}

/*
	~GameGUI - Destructor, this would be called when the application is closing,
	it will clean up all memory for the GUI, including the cursor and all
	ScreenGUIs.
*/
GameGUI::~GameGUI()
{
	// TODO - GO THROUGH SCREENS AND DELETE ALL CONTENTS
	delete customCursor;
}

/*
	addRenderItemsToRenderList - This method cascades this call to the
	current ScreenGUI. That object will add the appropriate items to
	the render list. This method should be called once per frame to make
	sure the current GUI is always rendered.

	Make sure the mode you provide matches the index of this GUI's screen.
	If an illegal mode value is provided, an exception is thrown.
*/
void GameGUI::addRenderItemsToRenderList(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameState gameState = gsm->getCurrentGameState();
	
	GameGraphics *graphics = game->getGraphics();
	RenderList *guiRenderList = graphics->getGUIRenderList();

	if (gsm->isAppActive())
	{
		// CASCADE THIS CALL TO THE APPROPRIATE SCREEN
		screens.at(gameState)->addRenderItemsToRenderList(guiRenderList);
	}

	// CASCADE THIS CALL TO THE CURSOR, IF THERE IS ONE
	if (customCursor != NULL)
		customCursor->addRenderItemToRenderList(guiRenderList);

}

/*
	addScreenGUI - This method adds a constructed ScreenGUI to the
	game's gui.
*/
void GameGUI::addScreenGUI(GameState gameState, ScreenGUI *screenToAdd)
{
	screens[gameState] = screenToAdd;
}

/*
	checkCurrentScreenForAction - This method should be called
	once per frame. It checks with the current screen to see if
	a button event needs to be fired, and fires it if necessary.

	If a mode is provided that does not match a ScreenGUI index,
	an exception is thrown.
*/
bool GameGUI::checkCurrentScreenForAction(Game *game)
{
	// IF THE GAME STATE DOESN'T HAVE A CORRESPONDING,
	// SCREEN DON'T DO ANYTHING
	GameStateManager *gsm = game->getGSM();
	GameState gameState = gsm->getCurrentGameState();
	ScreenGUI *gui = screens[gameState];
	if (gui != NULL)
	{
		// CASCADE THE CHECK TO THE CURRENT SCREEN
		return gui->fireButtonCommand(game);
	}
	return false;
}

/*
	registerButtonEventHandler - This method tells all of the screens in
	this gui the class that will handle button click events.
*/
void GameGUI::registerButtonEventHandler(ButtonEventHandler *eventHandler)
{
	map<GameState,ScreenGUI*>::iterator it = screens.begin();
	while (it != screens.end())
	{
		// NOTE THAT THE "second" VALUE IN A MAP ELEMENT IS THE DATA,
		// THE "first" WOULD BE THE KEY
		(*it).second->registerButtonEventHandler(eventHandler);
		it++;
	}
}

/*
	updateGUIState - This method sends mousePoint, the current cursor
	location, to the current GUI screen such that all of its button
	states can be updated. This method should be called once per frame
	after mouse position has been updated but before we check for fired
	events.

	If an illegal mode is provided, meaning a screen index that does not
	exist, an exception is thrown.
*/
void GameGUI::updateGUIState(long mouseX, long mouseY, GameState gameState)
{
	ScreenGUI *gui = screens[gameState];
	if (gui != NULL)
	{
		// LET THE CURRENT SCREEN UPDATE IT'S BUTTON STATES
		gui->updateAllButtons(mouseX, mouseY);
	}
}