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
	moveState = MS_STOP;
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
	if (fixturesToDestroy.size() > 0)
		destroyFixtures();

	// UPDATE PLAYER SPRITE'S POSITION
	movePlayer();
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
	for (b2Fixture* f = playerBody->GetFixtureList(); f; f = f->GetNext())
		if (f->IsSensor()) f->SetUserData((void*)PLAYER_SENSOR);

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
	}
	tempBodies.clear();

	json.getBodiesByName("Rock", tempBodies);
	spriteType = spriteManager->getSpriteType(CUBE_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);
		rocks.push_back(tempBodies[i]);

		// TODO: set any body definitions here
		for (b2Fixture* f = tempBodies[i]->GetFixtureList(); f; f = f->GetNext())
			f->SetUserData((void*)ROCK_SENSOR);
	}
	tempBodies.clear();

	json.getBodiesByName("Platform", tempBodies);
	spriteType = spriteManager->getSpriteType(PLATFORM_SPRITE);
	for (int i = 0; i < tempBodies.size(); i++)
	{
		loadLifelessObject(game, spriteType, tempBodies[i]);
		platforms.push_back(tempBodies[i]);

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

void Physics::movePlayer(void)
{
	b2Vec2 vel = playerBody->GetLinearVelocity();
	float desiredVel = 0;
	switch (moveState)
	{
		case MS_LEFT:  desiredVel = b2Max(vel.x - 5.0f, -MOVE_FORCE); break;
		case MS_STOP:  desiredVel = vel.x * 0.3f; break;
		case MS_RIGHT: desiredVel = b2Min(vel.x + 5.0f, MOVE_FORCE); break;
	}
	float velChange = desiredVel - vel.x;
	float force = playerBody->GetMass() * velChange / (1 / 60.0); //f = mv/t
	playerBody->ApplyForce(b2Vec2(force, 0), playerBody->GetWorldCenter(), true);
}

void Physics::jump(void)
{
	// To change velocity by 35
	float impulse = playerBody->GetMass() * JUMP_FORCE;
	playerBody->ApplyLinearImpulse(b2Vec2(0, impulse), playerBody->GetWorldCenter(), true);
}

// HELPER FUNCTIONS
void Physics::makePlayer(Game* game, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Player *playerSprite = spriteManager->getPlayer();
	this->player = playerSprite;
	player->numFootContacts = 0;
	player->isJumping = false;

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

void Physics::b2dToScreen(AnimatedSprite* sprite, float32 &x, float32 &y)
{
	x = ((x - sprite->getBoundingVolume()->getWidth() / settings.ratio / 2.0f) * settings.ratio) + (settings.worldWidth / 2.0f);
	y = ((-y - sprite->getBoundingVolume()->getHeight() / settings.ratio / 2.0f) * settings.ratio) + (settings.worldHeight / 2.0f);
}

// COLLISION HANDLING FUNCTIONS
void Physics::destroyFixtures()
{
	fixturesToDestroy.unique();
	list<b2Fixture*>::iterator it = fixturesToDestroy.begin();
	list<b2Fixture*>::iterator end = fixturesToDestroy.end();

	for (; it != end; ++it)
	{
		(*it)->GetBody()->SetType(b2_dynamicBody);
		(*it)->GetBody()->DestroyFixture((*it));
	}

	fixturesToDestroy.clear();
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
	
	// Is player on the ground?
	if ((int)fixtureA->GetUserData() == PLAYER_SENSOR
		|| (int)fixtureB->GetUserData() == PLAYER_SENSOR)
		player->numFootContacts++;

	if (player->numFootContacts >= 0) player->isJumping = false;

	// MOST CASES PLAYER WILL BE BODYB
	if (bodyB == playerBody)
		handleCollision(bodyA, fixtureA);

	else if (bodyA == playerBody)
		handleCollision(bodyB, fixtureB);
}

void Physics::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if ((int)fixtureA->GetUserData() == PLAYER_SENSOR
		|| (int)fixtureB->GetUserData() == PLAYER_SENSOR)
		player->numFootContacts--;
}

void Physics::handleCollision(b2Body* body, b2Fixture* fixture)
{
	// FALLING BODIES - PLAYER ENTERS IN CONTACT WITH THEM
	if ((int)fixture->GetUserData() == ROCK_SENSOR)
	{
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
			fixturesToDestroy.push_back(fixture);
	}

	// PLAYER WAS HIT BY AN ENEMY
	if (body->GetType() == b2_dynamicBody && body->IsBullet()) {
		// TODO: TAKE DAMAGE
		int health = player->getHealth();
		player->takeDamage(DAMAGE);
		health = player->getHealth();
		gameAudio->playSoundFX(XACT_WAVEBANK_SOUNDS_TAKINGDAMAGESOUND);
	}
}