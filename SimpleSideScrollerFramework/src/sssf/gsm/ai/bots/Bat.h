#pragma once

#include "sssf\gsm\physics\Physics.h"
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\game\Game.h"
#include <Box2D\Box2D.h>

class Bat : public Bot
{
public:
	b2Vec2 basePosition;
	float horizontalRange;
	float verticalRange;
	float speed;

	// Methods inherited from Bot.h
	virtual void think(Game* game);
	virtual Bot* clone();
};