/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Overlayimage.h

	An OverlayImage is just a simple structure for storing information
	about some static image in a game. It might be a background image
	in a gui or for static objects in a game world.
*/

#pragma once
#include "sssf_VS\stdafx.h"

struct OverlayImage
{
	int imageID;	// INDEX OF IMAGE IN TEXTURE MANAGER
	int x;			// X LOCATION
	int y;			// Y LOCATION
	int z;			// Z LAYER
	int alpha;		// TRANSPARENCY
	int width;		// TEXTURE WIDTH TO USE
	int height;		// TEXTURE HEIGHT TO USE
};