
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\ai\LifelessObject.h"
#include <Box2D\Box2D.h>

static const int		STOP = 0;
static const int		RIGHT = 1;
static const int		LEFT = 2;
static const int		JUMP = 3;
static const int		BOX_SPRITE = 1;

struct Settings
{
	Settings()
	{
		hz = 60.0f;
		ratio = 64.0f;
		velocityIterations = 8;
		positionIterations = 2;
		moveState = STOP;
		gravity.Set(0.0f, -10.0f);
	}

	float32 hz;
	float32 ratio;
	int32 velocityIterations;
	int32 positionIterations;
	
	float32 playerOffsetX;
	float32 playerOffsetY;
	b2Vec2 gravity;

	float32 worldWidth;
	float32 worldHeight;

	int moveState;
};

class AnimatedSprite;

class Physics : public b2ContactListener
{
public:
	// BOX2D STUFF
	b2World* world;
	b2Body* playerBody;
	AnimatedSprite* player;
	vector<b2Body*> b2Objects;
	vector<LifelessObject*> objects;
	Settings settings;

	// CONSTRUCTOR/DESTRUCTOR
	Physics();
	~Physics();

	// INLINE GET/SET METHOD

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp
	void update(Game* game);
	void createPlayer(AnimatedSprite* initPlayer);
	void loadScene(Game* game, const char* level);
	void setPlayerProperties(AnimatedSprite* initPlayer);
	void movePlayer(const int);

	// HELPER METHODS DEFINED INSIDE Physics.cpp
private:
	void Box2DToScreen(float32 &, float32 &);
	void b2ToScreen(AnimatedSprite* sprite, float32 &x, float32 &y);
	LifelessObject* makeLifelessObject(Game* game, AnimatedSpriteType* lifeLessType, float initX, float initY);
};
