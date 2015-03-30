/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TiledLayer.h

	A TiledLayer manages a tiled background, which is basically
	a grid of images that together create a game background.
	TiledLayers are packed, meaning there is a tile in every
	grid cell.
*/
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\world\WorldLayer.h"
#include "sssf\gui\Viewport.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\Physics.h"

// THIS CLASS IS-A WorldLayer

class TiledLayer: public WorldLayer
{
protected:
	// NOTE: tileLayout stores a grid of Tiles (rows X columns),
	// but does so using a one-dimensional vector. So, when
	// accessing and changing tiles in this data structure,
	// rows and columns must be converted into indices.
	vector<Tile*> *tileLayout;
	int columns;
	int rows;
	int tileWidth;
	int tileHeight;
	int layerWidth;
	int layerHeight;
	int z;
	// NOTE: z IS HERE IN CASE WE WANT TO LAYER SPRITES
	// IN FRONT OF OR BEHIND LAYERS IN A GAME WORLD

	// HELPER METHODS
	void initOverlappingCellRange(AABB *aabb, int &startCol, int &endCol, int &startRow, int &endRow);

public:
	// INLINED ACCESSOR METHODS
	int getColumns()			{ return columns;		}
	int getRows()				{ return rows;			}
	int getTileHeight()			{ return tileHeight;	}
	int getTileWidth()			{ return tileWidth;		}
	int getLayerWidth()			{ return layerWidth;	}
	int getLayerHeight()		{ return layerHeight;	}
	int getZ()					{ return z;				}

	// INLINED MUTATOR METHOD
	void setZ(int initZ)		{ z = initZ;			}

	// METHODS DEFINED IN TiledLayer.cpp
	TiledLayer(	int initColumns,	int initRows, 
				int initTileWidth,	int initTileHeight, 
				int initZ, 
				bool initCollidableTiles, 
				int initWorldWidth, int initWorldHeight);
	~TiledLayer();
	void	addRenderItemsToRenderList(RenderList *renderList, Viewport *viewport);
	void	addTile(Tile *initTile);
	int		calculateAndSetLayerHeight();
	int		calculateAndSetLayerWidth();
	int		calculateViewportEndColumn(int viewportX, int viewportWidth);
	int		calculateViewportEndRow(int viewportY, int viewportHeight);
	int		calculateViewportStartColumn(int viewportX, int viewportWidth);
	int		calculateViewportStartRow(int viewportY, int viewportHeight);
	Tile*	getTile(int row, int column);
	void	init(	int initColumns,	int initRows, 
					int initTileWidth,	int initTileHeight, 
					int initZ, 
					bool collidableTiles, 
					int worldWidth, int worldHeight);
	void	setTile(Tile *initTile, int row, int column);
	void	findTileCollisionsForSprite(	Physics *physics,
											CollidableObject *dynamicObject);
	bool	willSpriteCollideOnTile(	Physics *physics,
										CollidableObject *dynamicObject);

};