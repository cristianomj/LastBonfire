
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
		velocityIterations = 8;
		positionIterations = 2;
	}

	float32 hz;
	float32 ratio;
	int32 velocityIterations;
	int32 positionIterations;
	
	float32 playerOffsetX;
	float32 playerOffsetY;
};

class AnimatedSprite;

class Physics : public b2ContactListener
{
public:
	// BOX2D STUFF
	b2World* world;
	b2Body* playerBody;
	AnimatedSprite* player;
	Settings settings;

	// CONSTRUCTOR/DESTRUCTOR
	Physics();
	~Physics();

	// INLINE GET/SET METHOD

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp
	void update(Game* game);
	void createPlayer(AnimatedSprite* initPlayer);
	void loadScene(const char* level);
	void setPlayerProperties(AnimatedSprite* initPlayer);

	// HELPER METHODS DEFINED INSIDE Physics.cpp
private:
	void Box2DToScreen(float32 &, float32 &);
};
