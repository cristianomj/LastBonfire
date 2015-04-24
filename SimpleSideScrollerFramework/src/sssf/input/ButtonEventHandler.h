/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	ButtonEventHandler.h

	This class provides a framework for button event handlers. Each Button
	in a GameGUI have a ButtonEventHandler, but what that handler does
	is up to the individual game application.

	Button responses should be defined inside a child class' handleButtonEvents
	method. Since that method is passed the Game object, it has access to
	everything and can quite literally do anything it wants to the game.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class Game;

class ButtonEventHandler
{
protected:
	// INLINED CONSTRUCTOR
	ButtonEventHandler(){}

	// INLINED DESTRUCTOR
	~ButtonEventHandler(){}

public:
	// METHOD DEFINED IN GAME-SPECIFIC CHILD CLASSES
	virtual void handleButtonEvents(Game *game, wstring command) = 0;
};