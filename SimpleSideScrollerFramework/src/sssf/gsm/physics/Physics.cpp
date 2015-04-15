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

	world->Step(timeStep, settings.velocityIterations, settings.positionIterations);

	//for (b2Body *b = world->GetBodyList(); b; b = b->GetNext())
	//{
	//	if (b->GetUserData() != NULL) {

	//		// MOVE THE PLAYER SPRITE
	//		float32 px = b->GetPosition().x;
	//		float32 py = b->GetPosition().y;
	//		// Convert box2d coordinates to screen coordinates
	//		b2ToScreen(player, px, py);
	//		// Change sprite position
	//		player->getPhysicalProperties()->setPosition(px, py);
	//	}
	//}

	// MOVE THE PLAYER SPRITE
	float32 px = playerBody->GetPosition().x;
	float32 py = playerBody->GetPosition().y;
	// Convert box2d coordinates to screen coordinates
	b2ToScreen(player, px, py);
	// Change sprite position
	player->getPhysicalProperties()->setPosition(px, py);

	// MOVE LIFELESS OBJECTS
	for (int i = 0; i < b2Objects.size(); i++)
	{
		float32 ox = b2Objects[i]->GetPosition().x;
		float32 oy = b2Objects[i]->GetPosition().y;

		b2ToScreen(objects[i], ox, oy);

		objects[i]->getPhysicalProperties()->setPosition(ox, oy);
	}
	
}

void Physics::loadScene(Game* game, const char* level)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();

	settings.worldWidth = game->getGSM()->getWorld()->getWorldWidth();
	settings.worldHeight = game->getGSM()->getWorld()->getWorldHeight();

	b2dJson json;
	string errorMsg;
	world = json.readFromFile(level, errorMsg);
	world->SetGravity(settings.gravity);

	playerBody = json.getBodyByName("Player");
	playerBody->GetFixtureList()->SetFriction(0.0f);
	playerBody->SetUserData(player);
	playerBody->SetGravityScale(20);

	float32 x = playerBody->GetPosition().x;
	float32 y = playerBody->GetPosition().y;

	b2ToScreen(player, x, y);

	player->getPhysicalProperties()->setPosition(x, y);

	// LOAD ALL OBJECTS
	json.getBodiesByName("Box", b2Objects);
	AnimatedSpriteType *object = spriteManager->getSpriteType(BOX_SPRITE);
	for (int i = 0; i < b2Objects.size(); i++)
	{
		objects.push_back(makeLifelessObject(game, object, 0, 0));
		float32 x = b2Objects[i]->GetPosition().x;
		float32 y = b2Objects[i]->GetPosition().y;
		
		b2ToScreen(objects[i], x, y);
		objects[i]->getPhysicalProperties()->setPosition(x, y);

		// SET BODY DEFINITIONS
		b2Objects[i]->GetFixtureList()->SetFriction(1.0f);
		b2Objects[i]->SetGravityScale(10);
		b2Objects[i]->GetFixtureList()->SetDensity(10.0f);
	}
}

void Physics::Box2DToScreen(float32 &x, float32 &y)
{
	x = ((x - settings.playerOffsetX) * settings.ratio) + (1600.0f / 2.0f);
	y = ((-y - settings.playerOffsetY) * settings.ratio) + (768.0f / 2.0f);
}

void Physics::b2ToScreen(AnimatedSprite* sprite, float32 &x, float32 &y)
{
	x = ((x - sprite->getBoundingVolume()->getWidth() / settings.ratio / 2.0f) * settings.ratio) + (settings.worldWidth / 2.0f);
	y = ((-y - sprite->getBoundingVolume()->getHeight() / settings.ratio / 2.0f) * settings.ratio) + (settings.worldHeight / 2.0f);
}

void Physics::setPlayerProperties(AnimatedSprite* initPlayer)
{
	player = initPlayer;

	settings.playerOffsetX = player->getBoundingVolume()->getWidth() / settings.ratio / 2.0f;
	settings.playerOffsetY = player->getBoundingVolume()->getHeight() / settings.ratio / 2.0f;
}

void Physics::movePlayer(const int moveState)
{
	settings.moveState = moveState;
	b2Vec2 vel = playerBody->GetLinearVelocity();
	float desiredVelX = 0, desiredVelY = 0;
	switch (moveState)
	{
	case LEFT:  desiredVelX = -10; break;
	case STOP:  desiredVelX = 0; break;
	case RIGHT: desiredVelX = 10; break;
	case JUMP:  desiredVelY = 400; break;
	}
	float velChangeX = desiredVelX - vel.x;
	float velChangeY = desiredVelY - vel.y;
	float impulseX = playerBody->GetMass() * velChangeX; //disregard time factor
	float inpulseY = playerBody->GetMass() * velChangeY;
	playerBody->ApplyLinearImpulse(b2Vec2(impulseX, velChangeY), playerBody->GetWorldCenter(), true);
}

LifelessObject* Physics::makeLifelessObject(Game *game, AnimatedSpriteType *lifeLessType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();

	LifelessObject *object = new LifelessObject();

	PhysicalProperties *pp = object->getPhysicalProperties();
	pp->setPosition(initX, initY);
	object->setSpriteType(lifeLessType);

	object->setCurrentState(L"IDLE");
	object->setAlpha(255);
	spriteManager->addObject(object);
	object->affixTightAABBBoundingVolume();

	return object;
}