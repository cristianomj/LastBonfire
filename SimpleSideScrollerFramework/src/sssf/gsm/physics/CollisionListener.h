#pragma once

#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"

class CollisionListener
{
public:
	virtual void respondToCollision(Game *game, Collision *collision)=0;
};