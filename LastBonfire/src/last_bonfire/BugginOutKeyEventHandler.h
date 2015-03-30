/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugginOutKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "last_bonfire_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"

class Game;

class BugginOutKeyEventHandler : public KeyEventHandler
{
public:
	BugginOutKeyEventHandler()		{}
	~BugginOutKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
};