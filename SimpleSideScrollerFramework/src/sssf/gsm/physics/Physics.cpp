/*
	Author: Cristiano Miranda

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
#include "sssf\gsm\rubestuff\b2dJson.h"
#include <vector>
#include <list>

/*
	The constructor initializes the data structures and loads
	the necessary ones with recyclable objects for collision tests.
*/
Physics::Physics()
{
	settings = Settings();
}

Physics::~Physics()
{
	delete world;
	world = NULL;
}

void Physics::update(Game* game)
{	
	float32 timeStep = 1.0f / settings.hz;

	for (b2Body *b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {

			float32 px = b->GetPosition().x;
			float32 py = b->GetPosition().y;

			Box2DToScreen(px, py);

			player->getPhysicalProperties()->setPosition(px, py);
		}
	}

	world->Step(timeStep, settings.velocityIterations, settings.positionIterations);
}

void Physics::loadScene(const char* level)
{
	b2dJson json;
	string errorMsg;
	world = json.readFromFile(level, errorMsg);

	playerBody = json.getBodyByName("Player");
	playerBody->SetUserData(player);

	float32 x = playerBody->GetPosition().x;
	float32 y = playerBody->GetPosition().y;

	Box2DToScreen(x, y);

	player->getPhysicalProperties()->setPosition(x, y);
}

void Physics::Box2DToScreen(float32 &x, float32 &y)
{
	x = ((x - settings.playerOffsetX) * settings.ratio) + (1600.0f / 2.0f);
	y = ((-y - settings.playerOffsetY) * settings.ratio) + (768.0f / 2.0f);
}

void Physics::setPlayerProperties(AnimatedSprite* initPlayer)
{
	player = initPlayer;

	settings.playerOffsetX = player->getBoundingVolume()->getWidth() / settings.ratio / 2.0f;
	settings.playerOffsetY = player->getBoundingVolume()->getHeight() / settings.ratio / 2.0f;

}