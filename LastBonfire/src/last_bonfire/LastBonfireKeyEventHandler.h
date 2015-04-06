#pragma once
#include "last_bonfire_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"

class Game;

class LastBonfireKeyEventHandler : public KeyEventHandler
{
public:
	LastBonfireKeyEventHandler()		{}
	~LastBonfireKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
};