
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include <Box2D\Box2D.h>

struct Settings
{
	Settings()
	{
		hz = 60.0f;
		ratio = 64.0f;
		velocityIterations = 6;
		positionIterations = 2;
		enableWarmStarting = true;
		enableContinuous = true;
		enableSleep = true;
	}

	float32 hz;
	float32 ratio;
	int32 velocityIterations;
	int32 positionIterations;
	bool enableWarmStarting;
	bool enableContinuous;
	bool enableSleep;
	
};

class AnimatedSprite;

class Physics : public b2ContactListener
{
public:
	// BOX2D STUFF
	b2World* world;
	b2Body* playerbody;
	AnimatedSprite* player;

	// CONSTRUCTOR/DESTRUCTOR
	Physics();
	~Physics();

	// INLINE GET/SET METHOD

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp
	void update(Game* game);
	void createPlayer(AnimatedSprite* initPlayer);

	// HELPER METHODS DEFINED INSIDE Physics.cpp
private:
	void Step(Settings* settings);
	void createWorld(Settings* settings);
	void createRoom(Settings* settings);
};