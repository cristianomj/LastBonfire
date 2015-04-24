/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SparseLayer.h

	A SparseLayer manages a sparse background, which is
	number of images that are placed in a world with
	space in between them. They are not packed into a grid
	and can be in any random arrangement. 
*/
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gui\OverlayImage.h"
#include "sssf\gui\Viewport.h"
#include "sssf\gsm\world\WorldLayer.h"

// THIS CLASS IS-A WorldLayer

class SparseLayer : public WorldLayer
{
private:
	// SPARSE TILES NEED NOT BE IN ANY ORDER OR PATTERNED ARRANGEMENT
	vector<OverlayImage*> *sparseTiles;

public:
	// INLINED ACCESSOR METHOD
	OverlayImage* getTile(int index)	{	return sparseTiles->at(index);	}

	// INLINED MUTATOR METHOD
	void addTile(OverlayImage *initTile)
	{	sparseTiles->push_back(initTile);	}
	void SparseLayer::setTile(OverlayImage *initTile, int index)
	{	sparseTiles->at(index) = initTile;	}

	// METHODS DEFINED IN SparseLayer.cpp
	SparseLayer();
	~SparseLayer();
	void			addRenderItemsToRenderList(RenderList *renderList,	Viewport *viewport);
	void			findTileCollisionsOverlappingSweptShape(	Physics *physics,
													CollidableObject *dynamicObject);
};