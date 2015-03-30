// LastBonfire.cpp : Defines the entry point for the application.
// Author: Cristiano Miranda

#include "last_bonfire_VS\stdafx.h"

// LastBonfire GAME INCLUDES
#include "last_bonfire\LastBonfire.h"
#include "last_bonfire\BugginOutButtonEventHandler.h"
#include "last_bonfire\BugginOutCollisionListener.h"
#include "last_bonfire\BugginOutDataLoader.h"
#include "last_bonfire\BugginOutKeyEventHandler.h"
#include "last_bonfire\BugginOutTextGenerator.h"


// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/

struct Nums
{
	int num1;
	int num2;
};
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *bugginOutGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	BugginOutDataLoader *bugginOutDataLoader = new BugginOutDataLoader();
	bugginOutDataLoader->initWinHandle(hInstance, nCmdShow);
	bugginOutGame->setDataLoader(bugginOutDataLoader);
	bugginOutDataLoader->loadGame(bugginOutGame, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	bugginOutDataLoader->loadGUI(bugginOutGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	BugginOutButtonEventHandler *bugginOutButtonHandler = new BugginOutButtonEventHandler();
	GameGUI *gui = bugginOutGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)bugginOutButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	BugginOutKeyEventHandler *bugginOutKeyHandler = new BugginOutKeyEventHandler();
	bugginOutGame->getInput()->registerKeyHandler((KeyEventHandler*)bugginOutKeyHandler);

	// THIS WILL HANDLE PHYSICS COLLISION EVENTS
	BugginOutCollisionListener *bugginOutCollisionListener = new BugginOutCollisionListener();
	bugginOutGame->getGSM()->getPhysics()->setCollisionListener(bugginOutCollisionListener);

	// START THE GAME LOOP
	bugginOutGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)bugginOutGame->getOS();
	delete (WindowsInput*)bugginOutGame->getInput();
	delete (WindowsTimer*)bugginOutGame->getTimer();
	delete (DirectXGraphics*)bugginOutGame->getGraphics();
	delete (BugginOutTextGenerator*)bugginOutGame->getText()->getTextGenerator();
	delete bugginOutButtonHandler;
	delete bugginOutKeyHandler;
	delete bugginOutGame;

	// AND RETURN
	return 0;
}