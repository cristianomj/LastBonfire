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
	The constructor initializes all physics settings that will be used
	in our scene
*/
Physics::Physics()
{
	settings = Settings();
	world = NULL;
	playerBody = NULL;
}

Physics::~Physics()
{
	if (world != NULL) {
		delete world;
		world = NULL;
	}
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
	b2dToScreen(player, px, py);
	// Change sprite position
	player->getPhysicalProperties()->setPosition(px, py);

	// MOVE LIFELESS OBJECTS
	for (int i = 0; i < b2Objects.size(); i++)
	{
		float32 ox = b2Objects[i]->GetPosition().x;
		float32 oy = b2Objects[i]->GetPosition().y;
		// Convert box2d coordinates to screen coordinates
		b2dToScreen(objects[i], ox, oy);
		// Change sprite position and rotation
		objects[i]->getPhysicalProperties()->setPosition(ox, oy);
		objects[i]->setRotationInRadians(b2Objects[i]->GetAngle());
	}
	
}

void Physics::loadScene(Game* game, const char* level)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();

	settings.worldWidth = game->getGSM()->getWorld()->getWorldWidth();
	settings.worldHeight = game->getGSM()->getWorld()->getWorldHeight();

	// LOAD WORLD
	b2dJson json;
	string errorMsg;
	world = json.readFromFile(level, errorMsg);
	world->SetGravity(settings.gravity);

	// LOAD GROUND
	//ground = json.getBodyByName("Ground");

	// LOAD PLAYER
	playerBody = json.getBodyByName("Player");
	float32 x = playerBody->GetPosition().x;
	float32 y = playerBody->GetPosition().y;
	makePlayer(game, x, y);
	// Set player definitions
	playerBody->GetFixtureList()->SetFriction(0.0f);
	playerBody->SetGravityScale(20);
	//playerBody->SetUserData(player);

	// LOAD ALL OBJECTS
	json.getBodiesByName("Box", b2Objects);
	settings.boxes = b2Objects.size();
	AnimatedSpriteType *object = spriteManager->getSpriteType(BOX_SPRITE);
	for (int i = 0; i < b2Objects.size(); i++)
	{
		objects.push_back(makeLifelessObject(game, object, 0, 0));
		float32 x = b2Objects[i]->GetPosition().x;
		float32 y = b2Objects[i]->GetPosition().y;
		
		b2dToScreen(objects[i], x, y);
		objects[i]->getPhysicalProperties()->setPosition(x, y);

		// SET BODY DEFINITIONS
		b2Objects[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);

	}
	
	json.getBodiesByName("ScooterWheel", b2Objects);
	object = spriteManager->getSpriteType(WHEEL_SPRITE);
	for (int i = settings.boxes; i < b2Objects.size(); i++)
	{
		objects.push_back(makeLifelessObject(game, object, 0, 0));
		float32 x = b2Objects[i]->GetPosition().x;
		float32 y = b2Objects[i]->GetPosition().y;

		b2dToScreen(objects[i], x, y);
		objects[i]->getPhysicalProperties()->setPosition(x, y);
	}
	b2Objects.push_back(json.getBodyByName("Scooter"));
	object = spriteManager->getSpriteType(SCOOTER_SPRITE);
	objects.push_back(makeLifelessObject(game, object, 0, 0));
	x = b2Objects[b2Objects.size() - 1]->GetPosition().x;
	y = b2Objects[b2Objects.size() - 1]->GetPosition().y;
	b2dToScreen(objects[b2Objects.size() - 1], x, y);
	objects[b2Objects.size() - 1]->getPhysicalProperties()->setPosition(x, y);


	/*b2Objects[b2Objects.size() - 1]->GetFixtureList()->SetSensor(false);
	b2Objects[b2Objects.size() - 1]->SetType(b2_dynamicBody);*/
}

void Physics::b2dToScreen(AnimatedSprite* sprite, float32 &x, float32 &y)
{
	x = ((x - sprite->getBoundingVolume()->getWidth() / settings.ratio / 2.0f) * settings.ratio) + (settings.worldWidth / 2.0f);
	y = ((-y - sprite->getBoundingVolume()->getHeight() / settings.ratio / 2.0f) * settings.ratio) + (settings.worldHeight / 2.0f);
}

void Physics::movePlayer(const int moveState)
{
	settings.moveState = moveState;
	b2Vec2 vel = playerBody->GetLinearVelocity();
	float32 desiredVelX = 0.0f, desiredVelY = 0.0f;
	switch (moveState)
	{
		case LEFT:  desiredVelX = -settings.playerWalkingVel; break;
		case STOP:  desiredVelX = 0.0f; break;
		case RIGHT: desiredVelX = settings.playerWalkingVel; break;
		case JUMP:  desiredVelY = settings.playerJumpingVel; break;
	}
	float32 velChangeX = desiredVelX - vel.x;
	float32 velChangeY = desiredVelY - vel.y;
	float32 impulseX = playerBody->GetMass() * velChangeX; // disregard time factor
	float32 inpulseY = playerBody->GetMass() * velChangeY;
	playerBody->ApplyLinearImpulse(b2Vec2(impulseX, velChangeY), playerBody->GetWorldCenter(), true);
}

void Physics::makePlayer(Game* game, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSprite *playerSprite = spriteManager->getPlayer();
	this->player = playerSprite;

	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(PLAYER_SPRITE);
	playerSprite->setSpriteType(playerSpriteType);
	playerSprite->setAlpha(255);
	playerSprite->setCurrentState(IDLE);
	playerSprite->setRotationInRadians(0.0f);

	PhysicalProperties *playerProps = playerSprite->getPhysicalProperties();
	playerSprite->affixTightAABBBoundingVolume();
	b2dToScreen(player, initX, initY);
	playerSprite->getPhysicalProperties()->setPosition(initX, initY);

	settings.playerOffsetX = playerSprite->getBoundingVolume()->getWidth() / settings.ratio / 2.0f;
	settings.playerOffsetY = playerSprite->getBoundingVolume()->getHeight() / settings.ratio / 2.0f;
}

LifelessObject* Physics::makeLifelessObject(Game* game, AnimatedSpriteType *lifeLessType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();

	LifelessObject *object = new LifelessObject();

	PhysicalProperties *pp = object->getPhysicalProperties();
	pp->setPosition(initX, initY);
	object->setSpriteType(lifeLessType);
	object->setRotationInRadians(0.0f);

	object->setCurrentState(L"IDLE");
	object->setAlpha(255);
	spriteManager->addObject(object);
	object->affixTightAABBBoundingVolume();

	return object;
}