/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	KeyEventHandler.h

	This class provides a framework for key event handlers. It is assumed
	that each game application will have different mappings and different
	types of responses to key presses. That custom behavior must be programmed
	in cooperation with the indiviual game application.
	
	Child classes would put their custom responses inside the handleKeyEvents method,
	which is called by the GameInput method every frame. Since that method is given
	the Game object, it can access and manipulate everything in the game it needs
	in its response.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class Game;

class KeyEventHandler
{
protected:
	// INLINED CONSTRUCTOR
	KeyEventHandler(){}

	 // INLINED DESTRUCTOR
	~KeyEventHandler(){}

public:
	// METHOD DEFINED IN GAME-SPECIFIC CHILD CLASSES
	virtual void handleKeyEvents(Game *game)=0;
};