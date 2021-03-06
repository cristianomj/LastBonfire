#pragma once
#include "last_bonfire_VS\stdafx.h"

/*
	LastBonfire.h

	This file contains setup data for making the LastBonfire game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE BugginOut GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	W_LEVEL_1_DIR			= L"data/levels/CaveLevel/";
static const wstring	W_LEVEL_1_NAME			= L"CaveLevel.tmx";
static const wstring	W_LEVEL_1_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const char *		RUBE_LEVEL_1			= "data/levels/CaveLevel/CaveLevel.json";

static const wstring	W_LEVEL_2_DIR			= L"data/levels/SnowLevel/";
static const wstring	W_LEVEL_2_NAME			= L"SnowLevel.tmx";
static const wstring	W_LEVEL_2_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const char *		RUBE_LEVEL_2			= "data/levels/SnowLevel/SnowLevel.json";

static const wstring	W_LEVEL_3_DIR			= L"data/levels/Castle/";
static const wstring	W_LEVEL_3_NAME			= L"CastleLevel.tmx";
static const wstring	W_LEVEL_3_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const char *		RUBE_LEVEL_3			= "data/levels/Castle/Castle.json";

static const wstring	W_INIT_FILE				= L"data/LastBonfireInit.txt";
static const wstring	W_GUI_INIT_FILE			= L"data/LastBonfireGUI.txt";
static const string		W_DEBUG_FILE			= "DebugOutput.txt";

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	W_TITLE					= L"TITLE";
static const wstring	W_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT			= L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X		= L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y		= L"VIEWPORT_OFFSET_Y";
static const wstring	W_TOGGLE_OFFSET_Y		= L"TOGGLE_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED			= L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring	W_GO_TO_MM_COMMAND		= L"Go To Main Menu";
static const wstring	W_SPIN_LEFT_COMMAND		= L"Spin left";
static const wstring	W_SPIN_RIGHT_COMMAND	= L"Spin right";
static const wstring	W_START_COMMAND			= L"Start";
static const wstring	W_LOAD_COMMAND			= L"Load";
static const wstring	W_EXIT_COMMAND			= L"Exit";
static const wstring	W_QUIT_COMMAND			= L"Quit";
static const wstring	W_GREEN_CURSOR_PATH		= L"data/gui/cursor/flame_cursor.png";
static const wstring	W_RED_CURSOR_PATH		= L"data/gui/cursor/red_cursor.png";
static const wstring	W_SPLASH_SCREEN_PATH	= L"data/gui/overlays/last_bonfire_splash_screen_overlay.png";
static const wstring	W_MAIN_MENU_PATH		= L"data/gui/overlays/last_bonfire_main_menu_overlay.png";
static const wstring	W_START_IMAGE_PATH		= L"data/gui/buttons/start_game.png";
static const wstring	W_START_IMAGE_MO_PATH	= L"data/gui/buttons/start_game_mo.png";
static const wstring	W_LOAD_IMAGE_PATH		= L"data/gui/buttons/load_game.png";
static const wstring	W_LOAD_IMAGE_MO_PATH	= L"data/gui/buttons/load_game_mo.png";
static const wstring	W_EXIT_IMAGE_PATH		= L"data/gui/buttons/exit_game.png";
static const wstring	W_EXIT_IMAGE_MO_PATH	= L"data/gui/buttons/exit_game_mo.png";
static const wstring	W_QUIT_IMAGE_PATH		= L"data/gui/buttons/quit.png";
static const wstring	W_QUIT_IMAGE_MO_PATH	= L"data/gui/buttons/quit_mo.png";
static const wstring	W_LANTERN_OFF_PATH		= L"data/gui/buttons/lantern_off.png";
static const wstring	W_LANTERN_ON_PATH		= L"data/gui/buttons/lantern_on.png";
static const wstring	W_BONFIRE_LEFT_PATH		= L"data/gui/buttons/left_arrow.png";
static const wstring	W_BONFIRE_LEFT_MO_PATH	= L"data/gui/buttons/left_arrow_mo.png";
static const wstring	W_BONFIRE_RIGHT_PATH	= L"data/gui/buttons/right_arrow.png";
static const wstring	W_BONFIRE_RIGHT_MO_PATH = L"data/gui/buttons/right_arrow_mo.png";
static const wstring	W_BONFIRE_LEVEL_1_PATH	= L"data/gui/overlays/last_bonfire_level_one.png";
static const wstring	W_BONFIRE_LEVEL_2_PATH	= L"data/gui/overlays/last_bonfire_level_two.png";
static const wstring	W_BONFIRE_LEVEL_3_PATH	= L"data/gui/overlays/last_bonfire_level_three.png";
static const wstring	W_BONFIRE_SELECT_LEVEL	= L"data/gui/overlays/select_level_text";

// FOR OUR SPRITE
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
static const float		W_GRAVITY = 0.8f;
static const float		JUMPING_BOT_MIN_CYCLES = 30;
static const float		JUMPING_BOT_MAX_CYCLES = 120;
static const float		JUMPING_BOT_MAX_VELOCITY = 40;
static const float		PLAYER_INIT_X = 80.0f;
static const float		PLAYER_INIT_Y = 100.0f;
static const float		JUMP_SPEED = -24.0f;
static const float		PLAYER_SPEED = 6.0f;
static const int		MIN_FPS = 5;
static const int		MAX_FPS = 100;
static const int		FPS_INC = 1;

// USED FOR MOVEMENT
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int G_KEY = (unsigned int)'G';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;
const unsigned int CONTROL_KEY = VK_CONTROL;
const unsigned int ONE_KEY = (unsigned int)'1';
const unsigned int TWO_KEY = (unsigned int)'2';
const unsigned int THREE_KEY = (unsigned int)'3';

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
const unsigned int C_KEY = (unsigned int)'C';