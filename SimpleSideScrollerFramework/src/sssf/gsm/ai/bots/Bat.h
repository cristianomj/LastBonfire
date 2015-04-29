#pragma once

#include "sssf\gsm\physics\Physics.h"
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\game\Game.h"

class Bat : public Bot
{
	virtual void think(Game* game);
	virtual Bot* clone();
};