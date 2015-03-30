/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SparseLayer.cpp

	See SparseLayer.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\world\SparseLayer.h"
#include "sssf\gui\Viewport.h"

/*
	SparseLayer - Default Constructor, it initializes the data structure
	for storing our sparse tiles.
*/
SparseLayer::SparseLayer()
{
	sparseTiles = new vector<OverlayImage*>();
}

/*
	~SparseLayer - Destructor, it cleans up after the sparseTiles data structure.
*/
SparseLayer::~SparseLayer()
{
	delete sparseTiles;
}

/*
	addRenderItemsToRenderList - This method is called once per
	frame for each TiledLayer in a game world while the game is
	being played. It determines which Tiles in the layer are in
	the viewport, and creates a RenderItem for each one. It then
	adds each one to the provided render list.
*/
void SparseLayer::addRenderItemsToRenderList(RenderList *renderList,
											Viewport *viewport)
{
	// WE'LL USE THIS IN OUR CALCULATIONS
	float viewportX = (float)(viewport->getViewportX());
	float viewportY = (float)(viewport->getViewportY());

	// TILES THAT WE PUT IN THE RENDER LIST ARE GOING TO BE
	// RENDERED, SO WE ONLY WANT TO PUT TILES IN THE LIST
	// THAT OVERLAP THE CURRENT VIEWPORT. TO ENSURE PARALLAX
	// SCROLLING, SUBTRACT THE CORRECTED parallaxViewportX 
	// & parallaxViewportY FROM THE TILE X & Y VALUES TO 
	// SCALE SCROLLING.
	int adjustedX, adjustedY;
	vector<OverlayImage*>::iterator iterator;

	// GO THROUGH ALL THE TILES, STARTING WITH THE FIRST ONE
	iterator = sparseTiles->begin();
	while (iterator != sparseTiles->end())
	{
		// GET THE CURRENT TILE
		OverlayImage *tile = (*iterator);
		adjustedX = tile->x - (int)viewportX;
		adjustedY = tile->y - (int)viewportY;

		// IS THIS TILE VISIBLE?
		if (viewport->areWorldCoordinatesInViewport(
									(float)(tile->x),
									(float)(tile->y),
									tile->width,
									tile->height))
		{
			// IT'S VISIBLE, SO RENDER IT
			renderList->addRenderItem(	tile->imageID,
										adjustedX,
										adjustedY,
										tile->z,
										tile->alpha,
										tile->width,
										tile->height);
		}

		// ONTO THE NEXT ONE
		iterator++;
	}
}

void SparseLayer::findTileCollisionsOverlappingSweptShape(	Physics *physics,
													CollidableObject *dynamicObject)
{
	// CURRENTLY DOESN'T DO ANYTHING
}