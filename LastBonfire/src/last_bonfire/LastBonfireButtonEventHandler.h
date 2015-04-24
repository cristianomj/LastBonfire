#pragma once
#include "last_bonfire_VS\stdafx.h"
#include "sssf\input\ButtonEventHandler.h"

class Game;

class LastBonfireButtonEventHandler : public ButtonEventHandler
{
public:
	// AS-IS, THIS CLASS HAS NO DATA, SO THERE IS NOTHING
	// TO INITIALIZE OR DESTROY
	LastBonfireButtonEventHandler()	{}
	~LastBonfireButtonEventHandler()	{}

	// DEFINED INSIDE BugginOutButtonEventHandler.cpp
	void handleButtonEvents(Game *game,
		wstring command);
};