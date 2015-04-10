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
}

Physics::~Physics()
{
	delete world;
	world = NULL;
}

void Physics::update(Game* game)
{
	Settings* settings = new Settings();

	float x = player->getAlpha();

	float32 timeStep = 1.0f / 60.0f;

	world->Step(timeStep, settings->velocityIterations, settings->positionIterations);

	for (b2Body *b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {

			float32 px1 = (b->GetPosition().x * settings->ratio);
			float32 py1 = (b->GetPosition().y * settings->ratio);

			player->getPhysicalProperties()->setPosition(px1, py1);
		}
	}
}

void Physics::createWorld(Settings* settings)
{
	b2Vec2 gravity(0.0f, 10.0f);
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
	b2BodyDef roomDef;
	roomDef.position.Set(0, 0);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 21.0f);

	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(200.0f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);
}

void Physics::createPlayer(AnimatedSprite* initPlayer)
{
	player = initPlayer;
	float32 x = player->getPhysicalProperties()->getX();
	float32 y = player->getPhysicalProperties()->getY();
	float32 width = player->getBoundingVolume()->getWidth();
	float32 height = player->getBoundingVolume()->getHeight();

	// CREATE PLAYER BODY
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x / 64.0, y / 64.0);
	bodyDef.userData = player;
	b2Body* body = world->CreateBody(&bodyDef);

	b2Vec2 velocity(0.0f, 40.0f);
	body->SetLinearVelocity(velocity);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width / 64.0, height / 64.0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.6f;

	body->CreateFixture(&fixtureDef);
}