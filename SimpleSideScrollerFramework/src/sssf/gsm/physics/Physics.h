
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\ai\LifelessObject.h"
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
static const wstring	ATTACKING_RIGHT(L"ATTACKING_RIGHT");
static const wstring	ATTACKING_LEFT(L"ATTACKING_LEFT");
static const wstring	IDLE(L"IDLE");

struct Settings
{
	Settings()
	{
		hz = 60.0f;
		ratio = 64.0f;
		velocityIterations = 8;
		positionIterations = 2;
		gravity.Set(0.0f, -10.0f);
		playerWalkingVel = 10.0f;
		playerJumpingVel = 400.0f;
		moveState = STOP;
	}

	float32 hz;
	float32 ratio;
	int32 velocityIterations;
	int32 positionIterations;
	b2Vec2 gravity;

	// PLAYER SETTINGS
	float32 playerOffsetX;
	float32 playerOffsetY;
	float32 playerWalkingVel;
	float32 playerJumpingVel;
	int moveState;
	
	// WORLD SETTINGS
	float32 worldWidth;
	float32 worldHeight;

	int boxes;
	int wheels;
};

class AnimatedSprite;

class Physics : public b2ContactListener
{
public:
	// BOX2D STUFF
	b2World* world;
	b2Body* ground;
	b2Body* playerBody;
	AnimatedSprite* player;
	vector<b2Body*> b2Objects;
	vector<LifelessObject*> objects;
	set<b2Body*> scheduledForRemoval;

	vector<b2Body*> tempBodies;

	// LIFELESS OBJECTS
	set<b2Body*> lifelessObjects;
	

	Settings settings;

	// CONSTRUCTOR/DESTRUCTOR
	Physics();
	~Physics();

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp
	void update(Game* game);
	virtual void BeginContact(b2Contact* contact);
	void createPlayer(AnimatedSprite* initPlayer);
	void loadScene(Game* game, const char* level);
	void movePlayer(const int);

	// HELPER METHODS DEFINED INSIDE Physics.cpp
private:
	void b2dToScreen(AnimatedSprite* sprite, float32 &x, float32 &y);
	void makePlayer(Game* game, float initX, float initY);
	LifelessObject* makeLifelessObject(Game* game, AnimatedSpriteType* lifeLessType, float initX, float initY);
	void removeScheduledForRemoval(Game* game);
	void loadLifelessObject(Game* game, AnimatedSpriteType* spriteType, int i);
};
