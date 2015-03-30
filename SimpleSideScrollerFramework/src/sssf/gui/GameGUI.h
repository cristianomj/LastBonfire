/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGUI.h

	This class manages all data for the game gui, the cursor
	and the ScreenGUIs, which store the buttons and overlayed
	images.

	For example, the splash screen might be one screen, the main
	menu might be another, and the game as its running might be
	another.
*/
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Viewport.h"

class ButtonEventHandler;
class Button;
class Cursor;
class Game;
class GameInput;
class GameGraphics;
class RenderList;
class ScreenGUI;

class GameGUI
{
private:
	// THIS MAP STORES ALL OF OUR SCREENS FOR VARIOUS GAME STATES
	map<GameState, ScreenGUI*>	screens;

	// THIS IS OUR CURSOR, WITH WHICH WE CAN LOAD
	// WHATEVER IMAGES WE WANT
	Cursor				*customCursor;

	// THE VIEWPORT IS THE WINDOW ONTO THE GAME THAT WE LOOK THROUGH
	Viewport			viewport;

public:
	// INLINED ACCESSOR METHODS
	Cursor*		getCursor()						{ return customCursor;				}
	ScreenGUI*	getScreen(GameState gameState)	{ return screens.at(gameState);	}
	Viewport*	getViewport()					{ return &viewport;					}

	// INLINED MUTATOR METHOD
	void setCursor(Cursor *initCursor)
	{
		customCursor = initCursor;
	}

	// METHODS DEFINED IN GameGUI.cpp
	GameGUI();
	~GameGUI();
	void addRenderItemsToRenderList(Game *game);
	void addScreenGUI(GameState gameState, ScreenGUI *screenToAdd);
	bool checkCurrentScreenForAction(Game *game);
	void registerButtonEventHandler(ButtonEventHandler *eventHandler);
	void updateGUIState(long mouseX, long mouseY, GameState gameState);
};