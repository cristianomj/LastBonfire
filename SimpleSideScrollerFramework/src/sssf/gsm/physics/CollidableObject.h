#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\AABB.h"
#include "sssf\gsm\physics\PhysicalProperties.h"

// THESE ARE THE SWEEP AND PRUNE ORDERINGS
const unsigned int LEFT_EDGE = 0;
const unsigned int RIGHT_EDGE = 1;
const unsigned int TOP_EDGE = 2;
const unsigned int BOTTOM_EDGE = 3;

class CollidableObject
{
protected:
	AABB boundingVolume;
	bool currentlyCollidable;
	AABB sweptShape;
	unsigned int sweepAndPruneIndices[4];
	PhysicalProperties pp;
	bool onTileThisFrame;
	bool onTileLastFrame;
	unsigned int collisionEdge;

public:
	CollidableObject()	{}
	~CollidableObject()	{}

	// CollidableObject.cpp METHODS
	void updateSweptShape(float percentageOfFrameTimeRemaining);

	// INLINED METHODS
	bool				isCurrentlyCollidable()		{ return currentlyCollidable;	}
	bool				isOnTileThisFrame()			{ return onTileThisFrame;		}
	bool				wasOnTileLastFrame()		{ return onTileLastFrame;		}
	AABB*				getBoundingVolume()			{ return &boundingVolume;		}
	AABB*				getSweptShape()				{ return &sweptShape;			}
	PhysicalProperties* getPhysicalProperties()		{ return &pp;					}
	unsigned int		getCollisionEdge()			{ return collisionEdge;			}

	void				setCollisionEdge(unsigned int initCollisionEdge)
	{	collisionEdge = initCollisionEdge; }
	unsigned int		getSweepAndPruneIndex(unsigned int sweepAndPruneOrdering)		
	{ return sweepAndPruneIndices[sweepAndPruneOrdering];	}
	void				setSweepAndPruneIndex(unsigned int sweepAndPruneOrdering, unsigned int index)
	{ sweepAndPruneIndices[sweepAndPruneOrdering] = index;  }

	void setCurrentlyCollidable(bool initCurrentlyCollidable)
	{	currentlyCollidable = initCurrentlyCollidable; }
	void setOnTileThisFrame(bool initOnTileThisFrame)
	{	onTileThisFrame = initOnTileThisFrame; }
	void setOnTileLastFrame(bool initOnTileLastFrame)
	{	onTileLastFrame = initOnTileLastFrame; }

	void advanceOnTileStatus()
	{
		onTileLastFrame = onTileThisFrame;
		onTileThisFrame = false;
	}

};