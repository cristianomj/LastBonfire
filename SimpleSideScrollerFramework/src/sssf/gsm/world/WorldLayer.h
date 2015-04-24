/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WorldLayer.h

	This abstract class provides a framework for classes
	that manage data for one layer of texture rendering
	in a game world.

	Classes that extend this class would use their
	algorithms for rendering tiles. TiledLayer uses
	a traditional tiled background algorithm, SparseLayer
	uses a sparsely tiled background, and IsometricLayer
	uses an isometrically tiled background.

	This class has two instance variables, parallaxFactorX
	and parallaxFactorY, which should be used to control 
	parallax scrolling if desired. Parallax scrolling is 
	when a background scrolls at a slower pace then objects
	in the foreground, like a player, to give the illusion 
	of distance. 

	parallaxFactorX and parallaxFactorY are real numbers
	and represent a scaling of scrolling in the x and y
	axes. So, if parallaxFactorX is 2, scrolling in the x
	axis will move for this layer at half the speed as 
	those with factors of 1. The default for all layers
	is 1. If it is set to 0 or a negative number, parallax 
	scrolling is not used for this layer. If it is set to a
	positive number	less than 1, scrolling is sped up,
	by that factor.

	Generally speaking, the parallax factors for tiled
	layers should be calculated based on the dimensions
	of the layer relative to the world as a whole. For a
	sparse layer, this will have to be set since a sparse
	layer has no defined dimensions.
*/
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include <list>
#include <vector>

// FORWARD DECLARATIONS OF CLASSES USED BY WorldLayer
class RenderList;
class Viewport;
class Physics;

class WorldLayer
{
protected:
	bool  collidableTiles;
	int   worldWidth;
	int   worldHeight;

public:	
	// BY DEFAULT, TILES ARE NOT COLLIDABLE
	WorldLayer()	{	collidableTiles = false; }
	~WorldLayer()	{}

	// INLINED ACCESSOR METHOD
	bool  hasCollidableTiles() { return collidableTiles; }
	int getWorldWidth() { return worldWidth; }
	int getWorldHeight(){ return worldHeight; }

	// INLINED MUTATOR METHOD
	void setCollidableTiles(bool initCollidableTiles)
	{ collidableTiles = initCollidableTiles; }
	void setWorldWidth(int initWorldWidth)
	{ worldWidth = initWorldWidth; }
	void setWorldHeight(int initWorldHeight)
	{ worldHeight = initWorldHeight; }

	// METHOD DEFINED IN CHILD CLASSES
	virtual void addRenderItemsToRenderList(RenderList *renderList,	Viewport *viewport)=0;
	virtual void findTileCollisionsForSprite(	Physics *physics,
												CollidableObject *dynamicObject)=0;
	virtual bool willSpriteCollideOnTile(	Physics *physics,
											CollidableObject *dynamicObject)=0;
};