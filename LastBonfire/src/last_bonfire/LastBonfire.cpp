// LastBonfire.cpp : Defines the entry point for the application.
// Author: Cristiano Miranda

#include "last_bonfire_VS\stdafx.h"

// LastBonfire GAME INCLUDES
#include "last_bonfire\LastBonfire.h"
#include "last_bonfire\LastBonfireButtonEventHandler.h"
#include "last_bonfire\LastBonfireCollisionListener.h"
#include "last_bonfire\LastBonfireDataLoader.h"
#include "last_bonfire\lastBonfireKeyEventHandler.h"
#include "last_bonfire\LastBonfireTextGenerator.h"


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

//erica added
#include "pch.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

//end erica added

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
	Game *lastBonfire = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	LastBonfireDataLoader *lastBonfireDataLoader = new LastBonfireDataLoader();
	lastBonfireDataLoader->initWinHandle(hInstance, nCmdShow);
	lastBonfire->setDataLoader(lastBonfireDataLoader);
	lastBonfireDataLoader->loadGame(lastBonfire, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	lastBonfireDataLoader->loadGUI(lastBonfire, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	LastBonfireButtonEventHandler *lastBonfireButtonHandler = new LastBonfireButtonEventHandler();
	GameGUI *gui = lastBonfire->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)lastBonfireButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	LastBonfireKeyEventHandler *lastBonfireKeyHandler = new LastBonfireKeyEventHandler();
	lastBonfire->getInput()->registerKeyHandler((KeyEventHandler*)lastBonfireKeyHandler);

	// THIS WILL HANDLE PHYSICS COLLISION EVENTS
	LastBonfireCollisionListener *lastBonfireCollisionListener = new LastBonfireCollisionListener();
	//lastBonfire->getGSM()->getPhysics()->setCollisionListener(lastBonfireCollisionListener);

	// START THE GAME LOOP
	lastBonfire->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)lastBonfire->getOS();
	delete (WindowsInput*)lastBonfire->getInput();
	delete (WindowsTimer*)lastBonfire->getTimer();
	delete (DirectXGraphics*)lastBonfire->getGraphics();
	delete (LastBonfireTextGenerator*)lastBonfire->getText()->getTextGenerator();
	delete lastBonfireButtonHandler;
	delete lastBonfireKeyHandler;
	delete lastBonfire;

	// AND RETURN
	return 0;
}