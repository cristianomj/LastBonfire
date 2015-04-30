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
#include "sssf\audio\GameAudio.h"
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
	gameAudio = NULL;
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

	// MAKE ROCKS FALL
	if (rockSchedule.size() > 0)
		rockFall(game);

	// UPDATE PLAYER SPRITE'S POSITION
	float32 x = playerBody->GetPosition().x;
	float32 y = playerBody->GetPosition().y;
	// Convert box2d coordinates to screen coordinates
	b2dToScreen(player, x, y);
	// Change sprite position
	player->getPhysicalProperties()->setPosition(x, y);

	// UPDATE BOTS POSITIONS
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

void Physics::rockFall(Game* game)
{
	list<b2Body*>::iterator it = rockSchedule.begin();
	list<b2Body*>::iterator end = rockSchedule.end();

	for (; it != end; ++it)
	{
		/*b2Fixture* f;
		for (f = (*it)->GetFixtureList(); f; f = f->GetNext())
			if (f->IsSensor()) break;*/

		(*it)->SetType(b2_dynamicBody);
		/*(*it)->DestroyFixture(f);
		rocks.remove((*it));
		++it;*/
	}

	rockSchedule.clear();
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
		bots.erase(toRemove);
		// Remove sprite from render list
		spriteManager->removeLifelessObject((LifelessObject*)toRemove->GetUserData());
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

	list<b2Body*>::iterator it = rocks.begin();
	list<b2Body*>::iterator end = rocks.end();

	if (bodyA == playerBody)
	{
		// ROCK FALLING
		if (bodyB->GetType() == b2_kinematicBody)
		{
			for (; it != end; ++it)
			{
				if (bodyB == *it)
				{
					rockSchedule.push_back(bodyB);
				}
			}
		}

		if (bodyB->GetType() == b2_dynamicBody && bodyB->IsActive() && bodyB->IsBullet()) {
			gameAudio->playSoundFX(XACT_WAVEBANK_SOUNDS_TAKINGDAMAGESOUND);
		}
	}
	else if (bodyB == playerBody)
	{
		// ROCK FALLING
		if (bodyA->GetType() == b2_kinematicBody)
		{
			for (; it != end; ++it)
			{
				if (bodyA == *it)
				{
					rockSchedule.push_back(bodyA);
				}
			}
		}

		if (bodyA->GetType() == b2_dynamicBody && bodyA->IsActive() && bodyA->IsBullet()) {
			gameAudio->playSoundFX(XACT_WAVEBANK_SOUNDS_TAKINGDAMAGESOUND);
		}
			
	}
}

void Physics::loadScene(Game* game, const char* level)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	gameAudio = game->getAudio();

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
	playerBody->SetGravityScale(10);
	playerBody->SetUserData(player);

	// LOAD BOTS
	json.getBodiesByName("Bat", tempBodies);
	AnimatedSpriteType* spriteType = spriteManager->getSpriteType(BAT_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadBot(game, spriteType, tempBodies[i]);

		// TODO: set any body definitions here
	}
	tempBodies.clear(); // CLEAR TEMP BODIES

	// LOAD ALL OBJECTS
	json.getBodiesByName("Box", tempBodies);
	spriteType = spriteManager->getSpriteType(BOX_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}
	tempBodies.clear();

	json.getBodiesByName("Rock", tempBodies);
	spriteType = spriteManager->getSpriteType(ROCK_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);
		rocks.push_back(tempBodies[i]);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		tempBodies[i]->SetGravityScale(20);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}
	tempBodies.clear();

	json.getBodiesByName("Bone", tempBodies);
	spriteType = spriteManager->getSpriteType(BONE_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}
	tempBodies.clear();

	json.getBodiesByName("SkullPole", tempBodies);
	spriteType = spriteManager->getSpriteType(SKULLPOLE_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}
	tempBodies.clear();

	// LOAD SCOOTER WHEELS
	json.getBodiesByName("ScooterWheel", tempBodies);
	spriteType = spriteManager->getSpriteType(WHEEL_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}
	tempBodies.clear();

	// LOAD SCOOTER
	json.getBodiesByName("Scooter", tempBodies);
	spriteType = spriteManager->getSpriteType(SCOOTER_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);

		// TODO: set any body definitions here
		tempBodies[i]->GetFixtureList()->SetFriction(0.4f);
		//b2Objects[i]->GetFixtureList()->SetRestitution(0.6f);
	}
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

	playerSprite->affixTightAABBBoundingVolume();
	b2dToScreen(player, initX, initY);
	playerSprite->getPhysicalProperties()->setPosition(initX, initY);
}

void Physics::loadLifelessObject(Game* game, AnimatedSpriteType* spriteType, b2Body* body)
{
	// Create sprite and add it to sprite manager render list
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	LifelessObject* sprite = new LifelessObject();
	Physics *physics = game->getGSM()->getPhysics();

	sprite->setSpriteType(spriteType);
	spriteManager->addLifelessObject(sprite);
	sprite->affixTightAABBBoundingVolume();

	// Associate body with sprite
	body->SetUserData(sprite);

	// Add body to lifelessObject set
	lifelessObjects.insert(body);

	// Give sprite a starting position
	float32 x = body->GetPosition().x;
	float32 y = body->GetPosition().y;
	// box2d to world coordinate conversion
	b2dToScreen(sprite, x, y);
	sprite->getPhysicalProperties()->setPosition(x, y);
}

void Physics::loadBot(Game* game, AnimatedSpriteType* spriteType, b2Body* body)
{
	// Create sprite and add it to sprite manager render list
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Bat* sprite = new Bat();
	Physics *physics = game->getGSM()->getPhysics();

	sprite->setSpriteType(spriteType);
	spriteManager->addBot(sprite);
	sprite->affixTightAABBBoundingVolume();

	// Associate body with sprite
	body->SetUserData(sprite);

	// Add body to lifelessObject set
	bots.insert(body);

	// Give sprite a starting position
	float32 x = body->GetPosition().x;
	float32 y = body->GetPosition().y;
	// box2d to world coordinate conversion
	b2dToScreen(sprite, x, y);
	sprite->getPhysicalProperties()->setPosition(x, y);
}