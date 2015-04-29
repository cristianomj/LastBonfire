/*
	Author: Cristiano Miranda

	Physics.cpp

	See Physics.h for a class description.

	WORK BRANCH
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

	// REMOVE BODIES THAT WERE SCHEDULED TO BE REMOVED
	if (scheduledForRemoval.size() > 0)
		removeScheduledForRemoval(game);

	// UPDATE PLAYER SPRITE'S POSITION
	float32 x = playerBody->GetPosition().x;
	float32 y = playerBody->GetPosition().y;
	// Convert box2d coordinates to screen coordinates
	b2dToScreen(player, x, y);
	// Change sprite position
	player->getPhysicalProperties()->setPosition(x, y);

	// UPDATE LIFELESS OBJECTS' POSITION
	set<b2Body*>::iterator it = lifelessObjects.begin();
	set<b2Body*>::iterator end = lifelessObjects.end();
	for (; it != end; ++it)
	{
		b2Body* body = *it;
		LifelessObject* sprite = (LifelessObject*)body->GetUserData();
		// Get body's position
		x = body->GetPosition().x;
		y = body->GetPosition().y;
		// box2 to screen conversion
		b2dToScreen(sprite, x, y);
		// Update sprite position and rotation
		sprite->getPhysicalProperties()->setPosition(x, y);
		sprite->setRotationInRadians(body->GetAngle());
	}
}

void Physics::removeScheduledForRemoval(Game* game)
{
	SpriteManager* spriteManager = game->getGSM()->getSpriteManager();
	set<b2Body*>::iterator it = scheduledForRemoval.begin();
	set<b2Body*>::iterator end = scheduledForRemoval.end();
	for (; it != end; ++it)
	{
		b2Body* toRemove = *it;
		// Remove from bodies list
		lifelessObjects.erase(toRemove);
		// Remove sprite from render list
		spriteManager->removeObject((LifelessObject*)toRemove->GetUserData());
		// Destroy body
		world->DestroyBody(toRemove);
	}
	// Clear list for next time
	scheduledForRemoval.clear();
}

void Physics::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if (bodyA == playerBody)
	{
		if (bodyB->GetType() == b2_dynamicBody)
			scheduledForRemoval.insert(bodyB);
	}
	else if (bodyB == playerBody)
	{
		if (bodyA->GetType() == b2_dynamicBody)
			scheduledForRemoval.insert(bodyA);
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
	world->SetContactListener(this);

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
	playerBody->SetUserData(player);

	// LOAD ALL OBJECTS
	json.getBodiesByName("Box", tempBodies);
	AnimatedSpriteType* spriteType = spriteManager->getSpriteType(BOX_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, i);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}

	// CLEAR TEMP BODIES
	tempBodies.clear();

	// LOAD SCOOTER WHEELS
	json.getBodiesByName("ScooterWheel", tempBodies);
	spriteType = spriteManager->getSpriteType(WHEEL_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, i);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}

	// CLER TEMP BODIES
	tempBodies.clear();

	// LOAD SCOOTER
	json.getBodiesByName("Scooter", tempBodies);
	spriteType = spriteManager->getSpriteType(SCOOTER_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, i);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}

	// CLER TEMP BODIES
	tempBodies.clear();
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

void Physics::loadLifelessObject(Game* game, AnimatedSpriteType* spriteType, int i)
{
	// Create sprite and add it to sprite manager render list
	LifelessObject* sprite = makeLifelessObject(game, spriteType, 0, 0);
	// Associate body with sprite
	tempBodies[i]->SetUserData(sprite);

	// Add body to lifelessObject set
	lifelessObjects.insert(tempBodies[i]);

	// Give sprite a starting position
	float32 x = tempBodies[i]->GetPosition().x;
	float32 y = tempBodies[i]->GetPosition().y;
	// box2d to world coordinate conversion
	b2dToScreen(sprite, x, y);
	sprite->getPhysicalProperties()->setPosition(x, y);
}