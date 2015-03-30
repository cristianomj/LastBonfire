/*	
	Author: Richard McKenna

	Physics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\CollisionListener.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"

const float EPSILON = 0.00001f;
const float DEFAULT_GRAVITY = 0.3f;
const float BUFFER_BETWEEN_OBJECTS = 0.2f;
const float NUDGE_VELOCITY = 0.3f;
const float ENERGY_LOSS = 0.95f;

class Physics
{
//private:
public:
	// THIS IS APPLIED TO EVERY SPRITE EVERY FRAME
	float gravity;

	// THIS IS THE TIME IN THIS FRAME THAT HAS PASSED SO FAR,
	// NOT THAT ALL GAME OBJECTS WILL BE BROUGHT UP TO THIS MOMENT
	float currentCollisionTime;

	// THIS IS THE CUSTOM EVENT HANDLER FOR PROVIDING GAME-SPECIFIC
	// EVENTS TO COLLISIONS BETWEEN GAME OBJECTS
	CollisionListener *collisionListener;

	// WE USE LISTS BECAUSE WE ONLY REMOVE AND ADD FROM THE END FOR THESE 
	list<Collision*> recycledCollisions;
	list<CollidableObject*> recycledCollidableObjectsList;

	// WE USE A LIST BECAUSE WE'LL NEED TO PERIODICALLY REMOVE
	// FROM THE CENTER, AND WHILE WE DO SORT IT, WE DON'T SEARCH
	// IT FOR SINGLE ELEMENTS
	list<Collision*> activeCollisions;

	// SWEEP AND PRUNE STUFF
	map<unsigned int, vector<CollidableObject*>*> sortedSweptShapes;

	// THIS MAKES SURE WE AREN'T CHECKING A SLIGHLY PENETRATING COLLISION
	// PAIR OVER AND OVER AGAIN. THAT SCENARIO IS VERY HARD TO AVOID
	// IN A CONTINUOUS SYSTEM
	map<CollidableObject*, set<Tile*>> spriteToTileCollisionsThisFrame;

	// USED FOR TESTING PHYSICS BY TURNING IT ON AND OFF IN VARIOUS WAYS
	bool activated;
	bool activatedForSingleUpdate;

	// CONSTRUCDT/DESTRUCTOR
	Physics();
	~Physics();

	// INLINED GET/SET METHODS
	float				getGravity()					{ return gravity;					}
	CollisionListener*	getCollisionListener()			{ return collisionListener;			}
	bool				isActivated()					{ return activated;					}
	bool				isActivatedForSingleUpdate()	{ return activatedForSingleUpdate;	}
	void				activateForSingleUpdate()										{ activatedForSingleUpdate = true;				}
	void				setGravity(float initGravity)									{ gravity = initGravity;						}
	void				setCollisionListener(CollisionListener *initCollisionListener)	{ collisionListener = initCollisionListener;	}

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp - YOU ARE WELCOME TO ADD MORE OR CHANGE WHAT YOU LIKE
	void addCollidableObject(CollidableObject *collidableObjectToAdd);
	void addTileCollision(CollidableObject *dynamicObject, Tile *tile, float tileX, float tileY, float tileWidth, float tileHeight);
	void removeCollidableObject(CollidableObject *collidableObjectToRemove);
	void togglePhysics() { activated = !activated; }
	void update(Game *game);
	bool willSpriteCollideOnTile(CollidableObject *co, AABB *tileAABB);

	// HELPER METHODS DEFINED INSIDE Physics.cpp - YOU ARE WELCOME TO ADD MORE AS YOU SEE FIT
private:
	float calculateTimeUntilCollision(	CollidableObject *co1, 
										CollidableObject *co2, 
										unsigned int &co1Edge,
										unsigned int &co2Edge,
										float bufferBetweenObjects);
	float calculateTimeToImpact(	float lower, float lowerV,
									float upper, float upperV,
									float bufferBetweenObjects);
	float calculateTimeToStartOfCollision(	float lowerLower, float lowerUpper, float lowerV,
											float upperLower, float upperUpper, float upperV);
	void detectSpritesCollision(CollidableObject *co1, CollidableObject *co2);
	void determineXAxisCollisionEdge(AABB *aabb1, AABB *aabb2, unsigned &co1Edge, unsigned int &co2Edge);
	void determineYAxisCollisionEdge(AABB *aabb1, AABB *aabb2, unsigned &co1Edge, unsigned int &co2Edge);
	void getAllTileCollisionsForAGivenSprite(	World *world,
												CollidableObject *sprite,
												float percentageOfFrameRemaining);
	float getSide(CollidableObject *co, unsigned int ordering);
	void moveAllSpritesToEndOfFrame();
	void moveAllSpritesUpByTimeStep(float time);
	void moveAllSpritesUpToBufferedTimeOfCollision(Collision *collision);
	void performCollisionResponse(Collision *collision);
	void prepSpriteForCollisionTesting(World *world, CollidableObject *sprite);
	void retrieveSpriteCollisions();
	void removeActiveCOCollisions(CollidableObject *co);
	void removeCollidableObject(unsigned int ordering, CollidableObject *co);
	void reorderSweptShape(CollidableObject *co, unsigned int ordering, bool increasingDirection);
	void reorderCollidableObject(CollidableObject *co);
	void updateSweptShapeIndices();
	void updateSweptShapeIndices(vector<CollidableObject*> *sweptShapes, unsigned int ordering);
};

/*
	Provides a custom criteria for sorting for our swepts shapes when
	we want the sorted by left edge.
*/
struct SweptShapesComparitorByLeft
{
	inline bool operator()(CollidableObject *a, CollidableObject *b)
	{
		AABB *aSH = a->getSweptShape();
		AABB *bSH = b->getSweptShape();
		return aSH->getLeft() < bSH->getLeft();
	}
};

/*
	Provides a custom criteria for sorting for our swepts shapes when
	we want the sorted by right edge.
*/
struct SweptShapesComparitorByRight
{
	inline bool operator()(CollidableObject *a, CollidableObject *b)
	{
		AABB *aSH = a->getSweptShape();
		AABB *bSH = b->getSweptShape();
		return aSH->getRight() < bSH->getRight();
	}
};