/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameStateManager.cpp

	See GameStateManager.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\sprite\SpriteManager.h"

/*
	GameStateManager - Default Constructor, it starts the app at the
	splash screen with no level loaded.
*/
GameStateManager::GameStateManager()	
{
	spriteManager = new SpriteManager();
	currentGameState = GS_SPLASH_SCREEN;
	currentLevel = NO_LEVEL_LOADED;
}

/*
	~GameStateManager - This destructor should inform the game world and
	sprite managers to clean up all their allocated memory.
*/
GameStateManager::~GameStateManager()	
{
	delete spriteManager;
}

/*
	addGameRenderItemsToRenderList - This method adds all the world and sprite
	items in the viewport to the render list
*/
void GameStateManager::addGameRenderItemsToRenderList(Game *game)
{
	// FIRST THE STATIC WORL
	world.addWorldRenderItemsToRenderList(game);

	// THEN THE SPRITE MANAGER
	spriteManager->addSpriteItemsToRenderList(game);
}


/*
	goToGame - This method transitions the game application from the levl loading
	to the actualy game.
*/
void GameStateManager::goToGame()
{
	currentGameState = GS_GAME_IN_PROGRESS;
}

/*
	goToLoadLevel - This method transitions the game application from main menu
	to the level loading.
*/
void GameStateManager::goToLoadLevel()
{
	currentGameState = GS_STARTING_LEVEL;
}

/*
	goToMainMenu - This method transitions the game application from the splash
	screen to the main menu.
*/
void GameStateManager::goToMainMenu()
{
	currentGameState = GS_MAIN_MENU;
}

/*
	isAtSplashScreen - Used to test if this application is currently
	at the splash screen. This will dictate what to render, but also
	how to respond to user input.
*/
bool GameStateManager::isAtSplashScreen()
{
	return currentGameState == GS_SPLASH_SCREEN;
}

/*
	isAppActive - Used to test if this application is going to continue
	running for another frame. This will return true if the game app is
	not closing, false otherwise. Note that when the currentGameState
	becomes GS_EXIT_GAME, this will return true, and the game loop will
	end.
*/
bool GameStateManager::isAppActive()
{
	return currentGameState != GS_EXIT_GAME;
}

/*
	isGameInProgress - Used to test if the game is running right now, which
	means we have to execute all game logic.
*/
bool GameStateManager::isGameInProgress()
{
	return currentGameState == GS_GAME_IN_PROGRESS;
}

/*
	isGameLevelLoading - Used to test if the game is loading a level right now.
*/
bool GameStateManager::isGameLevelLoading()
{
	return currentGameState == GS_STARTING_LEVEL;
}

/*
	isWorldRenderable - Used to test if the game world should be rendered
	or not. Note that even if the game is paused, you'll likely still render
	the game.
*/
bool GameStateManager::isWorldRenderable()
{
	return (	(currentGameState == GS_GAME_IN_PROGRESS)
		||		(currentGameState == GS_PAUSED)
		||		(currentGameState == GS_GAME_OVER));
}

/*
	addLevelFileName - This method adds a level file name to the vector
	of all the level file names. Storing these file names allows us to
	easily load a desired level at any time.
*/
void GameStateManager::addLevel(wstring levelToAddName, wstring levelToAddFileNameAndPath)
{
	levelNames.push_back(levelToAddName);
	levelFileNamesWithRelativePath.push_back(levelToAddFileNameAndPath);
}

/*
	getLevelNum - Note that when our game app starts, we load all the level
	file names and this should never change. This method looks through those
	names and gets the index in the vector for a given level name. Note that
	we're using a little iterator/pointer arithmetic here since vectors
	guarantee they will store their contents in a continuous block of memory.
*/
unsigned int GameStateManager::getLevelNum(wstring levelName)
{
	return find(levelNames.begin(),levelNames.end(), levelName) - levelNames.begin();
}

/*
	loadLevel - This method changes the current level. This method should
	be called before loading all the data from a level file.
*/
void GameStateManager::loadLevel(Game *game, unsigned int initLevel)
{
	if ((initLevel > NO_LEVEL_LOADED) && (initLevel <= levelNames.size()))
	{
		if (currentLevel != NO_LEVEL_LOADED)
			unloadCurrentLevel();
		currentLevel = initLevel;
		wstring fileToLoad = levelFileNamesWithRelativePath[currentLevel];
		GameDataLoader *dataLoader = game->getDataLoader();
		dataLoader->loadWorld(game, fileToLoad);
	}
}

/*
	loadLevel - This is just an overladed version of our loadLevel method that
	uses the name of the level rather than its index. Some games may have non-
	linear levels, and so the developers may prefer to hold onto the level names
	rather than numbers.
*/
void GameStateManager::loadLevel(Game *game, wstring levelName)
{
	unsigned int levelIndex = getLevelNum(levelName);
	loadLevel(game, levelIndex);
}

/*
	shutdown - this method is called when the user wants to quit the
	application. This method updates the game state such that all
	world resources are released and the game loop does not iterate
	again.
*/
void GameStateManager::shutdown()
{
	// MAKE SURE THE GAME LOOP DOESN'T GO AROUND AGAIN
	currentGameState = GS_EXIT_GAME;

	// CLEAR LEFT OVER DATA
	if (isWorldRenderable())
		unloadCurrentLevel();
}

/*
	unloadCurrentLevel - This method removes all data from the World, recovering
	all used memory. It should be called first when a level is unloaded or changed. 
	If it is not called, an application runs the risk of having memory leaking,
	i.e. extra data sitting around that may slow the progam down. Or, if the app
	thinks a level is still active, it might add items to the render list using 
	image ids that have already been cleared from the GameGraphics' texture manager 
	for the world. That would likely result in an exception.
*/
void GameStateManager::unloadCurrentLevel()
{
	spriteManager->unloadSprites();
	world.unloadWorld();
}

/*
	update - This method should be called once per frame. It updates
	both the sprites and the game world. Note that even though the game
	world is for static data, should the user wish to put anything dynamic
	there (like a non-collidable moving layer), the updateWorld method
	is called.
*/
void GameStateManager::update(Game *game)
{
	spriteManager->update(game);
	world.update(game);
	if (physics.isActivated() || physics.isActivatedForSingleUpdate())
	{
		physics.update(game);
	}
}