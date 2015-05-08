#pragma once
/*
	Author: Cristiano Miranda
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\ai\LifelessObject.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\ai\bots\Bat.h"
#include "sssf\audio\GameAudio.h"
#include <Box2D\Box2D.h>

static const int		STOP = 0;
static const int		RIGHT = 1;
static const int		LEFT = 2;
static const int		JUMP = 3;
static const int		PLAYER_SPRITE = 0;
static const int		BOX_SPRITE = 1;
static const int		BALL_SPRITE = 2;
static const int		WHEEL_SPRITE = 3;
static const int		SCOOTER_SPRITE = 4;
static const int		BAT_SPRITE = 5;
static const int		BONE_SPRITE = 6;
static const int		SKULLPOLE_SPRITE = 7;
static const int		ROCK_SPRITE = 8;
static const int		PLATFORM_SPRITE = 9;
static const wstring	IDLE(L"IDLE");
static const wstring	MOVE_RIGHT(L"MOVE_RIGHT");
static const wstring	MOVE_LEFT(L"MOVE_LEFT");
static const wstring	JUMP_RIGHT(L"JUMP_RIGHT");
static const wstring	JUMP_LEFT(L"JUMP_RIGHT");

struct Settings
{
	Settings()
	{
		hz = 60.0f;
		ratio = 64.0f;
		velocityIterations = 8;
		positionIterations = 3;
	}

	float32 hz;
	float32 ratio;
	int32 velocityIterations;
	int32 positionIterations;
	
	// WORLD SETTINGS
	float32 worldWidth;
	float32 worldHeight;
};

enum MoveState {
	MS_STOP,
	MS_LEFT,
	MS_RIGHT,
	MS_JUMP
};

class AnimatedSprite;

class Physics : public b2ContactListener
{
public:

	// PHYSICS SETTINGS
	Settings settings;
	MoveState moveState;

	// CONSTRUCTOR/DESTRUCTOR
	Physics();
	~Physics();

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp
	void update(Game* game);
	virtual void BeginContact(b2Contact* contact);
	void createPlayer(AnimatedSprite* initPlayer);
	void loadScene(Game* game, const char* level);
	void movePlayer(void);
	void jump(void);

private:
	// BOX2D STUFF
	b2World* world;
	b2Body* ground;

	// PLAYER DATA
	b2Body* playerBody;
	AnimatedSprite* player;

	// USED TO TEMPORARILY STORE BODIES
	vector<b2Body*> tempBodies;

	// BOTS
	set<b2Body*> bots;
	// LIFELESS OBJECTS
	set<b2Body*> lifelessObjects;

	GameAudio* gameAudio;

	// USED TO REMOVE BODIES
	set<b2Body*> scheduledForRemoval;
	// ROCKS
	list<b2Body*> rocks;
	list<b2Fixture*> fixturesToDestroy;
	// FALLING PLATFORMS
	list<b2Body*> platforms;

	// HELPER METHODS DEFINED INSIDE Physics.cpp
	void b2dToScreen(AnimatedSprite* sprite, float32 &x, float32 &y);
	void makePlayer(Game* game, float initX, float initY);
	void removeScheduledForRemoval(Game* game);
	void loadLifelessObject(Game* game, AnimatedSpriteType* spriteType, b2Body* body);
	void loadBot(Game* game, AnimatedSpriteType* spriteType, b2Body* body);
	void destroyFixtures();
	void handleCollision(b2Body* body, b2Fixture* fixture);
};
