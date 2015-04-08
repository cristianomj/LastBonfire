
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include <Box2D\Box2D.h>

const float EPSILON = 0.00001f;
const float DEFAULT_GRAVITY = 0.3f;
const float BUFFER_BETWEEN_OBJECTS = 0.2f;
const float NUDGE_VELOCITY = 0.3f;
const float ENERGY_LOSS = 0.95f;

struct Settings
{
	Settings()
	{
		hz = 60.0f;
		ratio = 64.0f;
		velocityIterations = 6;
		positionIterations = 2;
		enableWarmStarting = 1;
		enableContinuous = 1;
		enableSleep = 1;
	}

	float32 hz;
	float32 ratio;
	int32 velocityIterations;
	int32 positionIterations;
	int32 enableWarmStarting;
	int32 enableContinuous;
	int32 enableSleep;
	
};

class Physics : public b2ContactListener
{
//private:
public:
	// BOX2D STUFF
	b2World *world;

	// CONSTRUCTOR/DESTRUCTOR
	Physics();
	~Physics();

	// INLINE GET/SET METHOD

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp
	void update(Game* game);

	// HELPER METHODS DEFINED INSIDE Physics.cpp
private:
	void Step(Settings* settings);
	void createWorld(Settings* settings);
	void createRoom(Settings* settings);
	void createPlayer(Settings* settings);
};