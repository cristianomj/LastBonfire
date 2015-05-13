#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"

class Player : public AnimatedSprite
{
public:
	Player() : health(6), numFootContacts(0), isJumping(false)	{}
	~Player()	{}

	int numFootContacts;
	bool isJumping;

	// Inline accessor methods
	int getHealth(void) const			{ return health;					}

	// Inline mutator methods
	void takeDamage(const int damage)	{ if(health > 0) health += damage;	}
	void restoreHealth(void)			{ health = 6;						}
private:
	int health;
};