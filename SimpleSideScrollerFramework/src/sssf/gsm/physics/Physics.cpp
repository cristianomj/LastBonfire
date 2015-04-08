/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Physics.cpp

	See Physics.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\world\SparseLayer.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\WorldLayer.h"
#include <vector>
#include <list>

/*
	The constructor initializes the data structures and loads
	the necessary ones with recyclable objects for collision tests.
*/
Physics::Physics()
{
	Settings* settings = new Settings();
	createWorld(settings);
	createRoom(settings);
	createPlayer(settings);
}

Physics::~Physics()
{
	delete world;
	world = NULL;
}

void Physics::update(Game* game)
{

}

void Physics::createWorld(Settings* settings)
{
	b2Vec2 gravity(0.0f, -10.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(settings->enableSleep);
	world->SetContinuousPhysics(settings->enableContinuous);
	world->SetWarmStarting(settings->enableWarmStarting);
}

void Physics::Step(Settings* settings)
{

}

void Physics::createRoom(Settings* settings)
{

}

void Physics::createPlayer(Settings* settings)
{

}