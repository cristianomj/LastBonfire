#include "last_bonfire_VS\stdafx.h"
#include "last_bonfire\LastBonfire.h"
#include "last_bonfire\LastBonfireKeyEventHandler.h"
#include "LastBonfireDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"
#include "sssf\audio\GameAudio.h"

/*
handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
key states since the last frame, which can allow us to respond to key presses, including when keys
are held down for multiple frames.
*/
void LastBonfireKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	Player *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();
	Viewport *viewport = game->getGUI()->getViewport();
	Physics* physics = game->getGSM()->getPhysics();
	GameAudio* gameAudio = game->getAudio();
	GameDataLoader* dataLoader = game->getDataLoader();

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = pp->getVelocityX();
		float vY = pp->getVelocityY();

		// YOU MIGHT WANT TO UNCOMMENT THIS FOR SOME TESTING,
		// BUT IN THIS ASSIGNMENT, THE USER MOVES VIA MOUSE BUTTON PRESSES
		
		// MOVE LEFT
		if (input->isKeyDown(A_KEY))
		{
 			vX = -PLAYER_SPEED;
			if (player->isJumping) player->setCurrentState(JUMP_LEFT);
			else player->setCurrentState(MOVE_LEFT);	// Animation State
			physics->moveState = MS_LEFT;		// Move State
		}
		// MOVE RIGHT
		else if (input->isKeyDown(D_KEY))
		{
			vX = PLAYER_SPEED;
			if (player->isJumping) player->setCurrentState(JUMP_RIGHT);
			else player->setCurrentState(MOVE_RIGHT);	// Animation State
			physics->moveState = MS_RIGHT;			// Move State
		}
		else if (input->isKeyDownForFirstTime(G_KEY))
		{
			viewport->toggleDebugView();
			game->getGraphics()->toggleDebugTextShouldBeRendered();
		}
		else // IDLE
		{
			vX = 0.0f;
			player->setCurrentState(IDLE);	// Animation State
			physics->moveState = MS_STOP;	// Move State
		}
		// JUMP
		if (input->isKeyDownForFirstTime(SPACE_KEY))
		{
			if (player->numFootContacts > 0)
			{
				// JUMP
				vY = JUMP_SPEED;
				player->isJumping = true;

				physics->moveState = MS_JUMP;
				physics->jump();

				if (vX > 0.0f) player->setCurrentState(JUMP_RIGHT);
				else if (vX < 0.0f) player->setCurrentState(JUMP_LEFT);

				gameAudio->playSoundFX(XACT_WAVEBANK_SOUNDS_JUMPSOUND);
			}
		}

		// NOW SET THE ACTUAL PLAYER VELOCITY
		pp->setVelocity(vX, vY);

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		if (input->isKeyDown(UP_KEY))
		{
			viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(DOWN_KEY))
		{
			viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(LEFT_KEY))
		{
			viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(RIGHT_KEY))
		{
			viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		Viewport *viewport = game->getGUI()->getViewport();
		if (viewportMoved)
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());

	}

	// LEVEL SELECTION CHEAT - CTR + LEVEL NUMBER
	if (input->isKeyDown(CONTROL_KEY)) {
		wstring LEVEL_TO_LOAD = L"";
		if (input->isKeyDownForFirstTime(ONE_KEY)) LEVEL_TO_LOAD = W_LEVEL_1_NAME;
		else if (input->isKeyDownForFirstTime(TWO_KEY)) LEVEL_TO_LOAD = W_LEVEL_2_NAME;
		else if (input->isKeyDownForFirstTime(THREE_KEY)) LEVEL_TO_LOAD = W_LEVEL_3_NAME;
		if (LEVEL_TO_LOAD != L"") {
			game->setCurrentLevelFileName(LEVEL_TO_LOAD);
			if (gsm->getCurrentGameState() != GS_GAME_IN_PROGRESS) game->startGame();
			else {
				gsm->unloadCurrentLevel();
				dataLoader->loadWorld(game, LEVEL_TO_LOAD);
			}
		}
	}
	// MUTE GAME
	if (input->isKeyDownForFirstTime(S_KEY)) 
	{
		gameAudio->setVolume(0.0f);
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}