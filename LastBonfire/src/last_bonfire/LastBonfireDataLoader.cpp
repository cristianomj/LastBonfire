#include "last_bonfire_VS\stdafx.h"

// LastBonfire GAME INCLUDES
#include "last_bonfire\LastBonfireDataLoader.h"
#include "last_bonfire\LastBonfireTextGenerator.h"
#include "last_bonfire\LastBonfire.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\ai\LifelessObject.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\audio\GameAudio.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

/*
loadGame - This method loads the setup game data into the game and
constructs all the needed objects for the game to work.
*/
void LastBonfireDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring, wstring> *properties = new map<wstring, wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *bugginOutOS = new WindowsOS(hInstance,
		nCmdShow,
		useFullscreen,
		titleProp,
		screenWidth, screenHeight,
		game);

	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *bugginOutGraphics = new DirectXGraphics(game);
	bugginOutGraphics->init(screenWidth, screenHeight);
	bugginOutGraphics->initGraphics(bugginOutOS, useFullscreen);
	bugginOutGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(bugginOutGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *bugginOutInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *bugginOutTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins((GameGraphics*)bugginOutGraphics,
		(GameInput*)bugginOutInput,
		(GameOS*)bugginOutOS,
		(GameTimer*)bugginOutTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	LastBonfireTextGenerator *lastBonfireTextGenerator = new LastBonfireTextGenerator();
	lastBonfireTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)lastBonfireTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
initColors - this helper method loads the color key, used for loading
images, and the font color, used for rendering text.
*/
void LastBonfireDataLoader::initColors(GameGraphics *graphics,
	map<wstring, wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
loadGUI - This method loads all the GUI assets described in the guiInitFile
argument. Note that we are loading all GUI art for all GUIs when the application
first starts. We'll learn a better technique later in the semester.
*/
void LastBonfireDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
loadLevel - This method should load the data the level described by the
levelInitFile argument in to the Game's game state manager.
*/
void LastBonfireDataLoader::loadWorld(Game *game, wstring levelInitFile)
{
	wstring LEVEL_DIR, LEVEL_NAME;
	const char * RUBE_LEVEL = NULL;
	int LEVEL_SOUND;
	// SELECT LEVEL
	if (levelInitFile == W_LEVEL_1_NAME) {
		LEVEL_DIR = W_LEVEL_1_DIR;
		LEVEL_NAME = W_LEVEL_1_NAME;
		RUBE_LEVEL = RUBE_LEVEL_1;
		LEVEL_SOUND = XACT_WAVEBANK_SOUNDS_CAVEMUSIC;
	}
	else if (levelInitFile == W_LEVEL_2_NAME) {
		LEVEL_DIR = W_LEVEL_2_DIR;
		LEVEL_NAME = W_LEVEL_2_NAME;
		RUBE_LEVEL = RUBE_LEVEL_2;
		LEVEL_SOUND = XACT_WAVEBANK_SOUNDS_SNOWMUSIC;
	}
	else if (levelInitFile == W_LEVEL_3_NAME) {
		LEVEL_DIR = W_LEVEL_3_DIR;
		LEVEL_NAME = W_LEVEL_3_NAME;
		RUBE_LEVEL = RUBE_LEVEL_3;
		LEVEL_SOUND = XACT_WAVEBANK_SOUNDS_CASTLEMUSIC;
	}

	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, LEVEL_DIR, LEVEL_NAME);

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LOAD RUBE LEVEL
	Physics *physics = game->getGSM()->getPhysics();
	physics->loadScene(game, RUBE_LEVEL);

	// LOAD LEVEL SOUND
	game->getAudio()->playBackgroundSound(LEVEL_SOUND);
}

/*
initBugginOutGUI - This method builds a GUI for the BugginOut Game application.
Note that we load all the GUI components from this method, including
the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void LastBonfireDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
}

/*
initCursor - initializes a simple little cursor for the gui.
*/
void LastBonfireDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(imageIDs,
		*(imageIDs->begin()),
		0,
		0,
		0,
		255,
		32,
		32);
	gui->setCursor(cursor);
}

/*
initSplashScreen - initializes the game's splash screen gui.
*/
void LastBonfireDataLoader::initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	splashScreenGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 1024;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	splashScreenGUI->addOverlayImage(imageToAdd);

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_LANTERN_OFF_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_LANTERN_ON_PATH);

	// BONFIRE BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		507,
		506,
		0,
		255,
		101,
		170,
		false,
		W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
initMainMenu - initializes the game's main menu gui.
*/
void LastBonfireDataLoader::initMainMenu(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_BONFIRE_LEVEL_1_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);
	
	imageID = guiTextureManager->loadTexture(W_BONFIRE_LEVEL_2_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 1024;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);
	
	imageID = guiTextureManager->loadTexture(W_BONFIRE_LEVEL_3_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 2048;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		400,
		700,
		0,
		255,
		200,
		50,
		false,
		W_EXIT_COMMAND);
	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);
	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		100,
		700,
		0,
		255,
		200,
		50,
		false,
		W_START_COMMAND);
	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// SPIN LEFT BUTTON
	normalTextureID = guiTextureManager->loadTexture(W_BONFIRE_LEFT_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_BONFIRE_LEFT_MO_PATH);
	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		350,
		0,
		255,
		128,
		128,
		false,
		W_SPIN_LEFT_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// SPIN RIGHT BUTTON
	normalTextureID = guiTextureManager->loadTexture(W_BONFIRE_RIGHT_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_BONFIRE_RIGHT_MO_PATH);
	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		1024 - 128,
		350,
		0,
		255,
		128,
		128,
		false,
		W_SPIN_RIGHT_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU, mainMenuGUI);
}

/*
initInGameGUI - initializes the game's in-game gui.
*/
void LastBonfireDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		200,
		100,
		false,
		W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS, inGameGUI);
}

/*
initViewport - initializes the game's viewport.
*/
void LastBonfireDataLoader::initViewport(GameGUI *gui, map<wstring, wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
}
