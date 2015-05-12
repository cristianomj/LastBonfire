#include "last_bonfire_VS\stdafx.h"
#include "last_bonfire\LastBonfire.h"
#include "last_bonfire\LastBonfireButtonEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\audio\GameAudio.h"
#include "sssf\audio\sounds.h"

void LastBonfireButtonEventHandler::handleButtonEvents(Game *game,
	wstring command)
{
	GameAudio* gameAudio = game->getAudio();

	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(W_START_COMMAND) == 0)
	{
		list<OverlayImage*>::iterator it = game->getGUI()->getScreen(GS_MAIN_MENU)->getOverlayIterator();
		int level = (*it)->x;

		if (level == 0) game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		else if (level == -1680) game->setCurrentLevelFileName(W_LEVEL_2_NAME);
		else if (level == -3360) game->setCurrentLevelFileName(W_LEVEL_3_NAME);
		game->startGame();
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		game->quitGame();
		game->getGSM()->unloadCurrentLevel();
		gameAudio->initAudio();
		gameAudio->playBackgroundSound(XACT_WAVEBANK_SOUNDS_BACKGROUND);
	}

	else if (command.compare(W_SPIN_RIGHT_COMMAND) == 0)
	{
		list<OverlayImage*>::iterator overlayIterator;
		overlayIterator = game->getGUI()->getScreen(GS_MAIN_MENU)->getOverlayIterator();
		if ((*overlayIterator)->x != (*overlayIterator)->width * -2) {

			// TODO: PLAY CLICK SOUND
			gameAudio->playSoundFX(XACT_WAVEBANK_SOUNDS_MENUSELECTIONCLICK);

			while (overlayIterator != game->getGUI()->getScreen(GS_MAIN_MENU)->getEndOfOverlayIterator())
			{
				OverlayImage *overlay = (*overlayIterator);
				overlay->x -= overlay->width;
				overlayIterator++;
			}
		}
	}
	else if (command.compare(W_SPIN_LEFT_COMMAND) == 0)
	{
		list<OverlayImage*>::iterator overlayIterator;
		overlayIterator = game->getGUI()->getScreen(GS_MAIN_MENU)->getOverlayIterator();
		if ((*overlayIterator)->x != 0) {

			// TODO: PLAY CLICK SOUND
			gameAudio->playSoundFX(XACT_WAVEBANK_SOUNDS_MENUSELECTIONCLICK);

			while (overlayIterator != game->getGUI()->getScreen(GS_MAIN_MENU)->getEndOfOverlayIterator())
			{
				OverlayImage *overlay = (*overlayIterator);
				overlay->x += overlay->width;
				overlayIterator++;
			}
		}
	}
}