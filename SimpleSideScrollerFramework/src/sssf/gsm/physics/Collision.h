#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\world\Tile.h"

class Collision
{
private:
	CollidableObject *co1;
	CollidableObject *co2;
	Tile *tile;
	unsigned int co1Edge;
	unsigned int co2Edge;
	bool collisionWithTile;
	float timeOfCollision;
	float startTimeOfXCollision;
	float endTimeOfXCollision;
	float startTimeOfYCollision;
	float endTimeOfYCollision;

public:
	Collision()	{}
	~Collision(){}

	// INLINED METHODS
	CollidableObject* getCO1() { return co1;				}
	CollidableObject* getCO2() { return co2;				}
	unsigned int getCO1Edge() { return co1Edge; }
	unsigned int getCO2Edge() { return co2Edge; }
	Tile* getTile() { return tile; }
	float getTimeOfCollision() { return timeOfCollision;	}
	float getStartTimeOfXCollision(){ return startTimeOfXCollision;	}
	float getStartTimeOfYCollision(){ return startTimeOfYCollision;	}
	bool  isCollisionWithTile() {return collisionWithTile; }

	void setCO1(CollidableObject *initCO1)	{ co1 = initCO1;				}
	void setCO2(CollidableObject *initCO2)	{ co2 = initCO2;				}
	void setCO1Edge(unsigned int initCO1Edge) { co1Edge = initCO1Edge; }
	void setCO2Edge(unsigned int initCO2Edge) { co2Edge = initCO2Edge; }
	void setCollisionWithTile(bool initCollisionWithTile) { collisionWithTile = initCollisionWithTile; }
	void setTile(Tile *initTile) { tile = initTile; }
	void setTimeOfCollision(float initTime) { timeOfCollision = initTime;	}
	void setStartTimeOfXCollision(float initTime){ startTimeOfXCollision = initTime;	}
	void setStartTimeOfYCollision(float initTime){ startTimeOfYCollision = initTime;	}
};

struct CollisionComparitor
{
	inline bool operator()(Collision *a, Collision *b)
	{
		return b->getTimeOfCollision() < a->getTimeOfCollision();
	}
};