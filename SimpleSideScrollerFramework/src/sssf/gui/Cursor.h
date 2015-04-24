/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Cursor.h

	This class is for storing information for a custom
	game cursor. It can store the image IDs of multiple
	images, as loaded into a texture manager, and so by
	changing the current cursor ID, it can change the 
	cursor to be rendered.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\graphics\RenderList.h"

// Forward declarations of classes used by Cursor
class Game;
class GameGraphics;
class GameInput;

class Cursor
{
private:
	// THESE ARE THE INDICES IN THE GUI TEXTURE MANAGER OF
	// ALL OF THE IMAGES WE CAN USE FOR THIS CURSOR
	vector<unsigned int> *imageIDs;

	// THIS IS THE INDEX OF THE IMAGE WE ARE CURRENTLY USING
	unsigned int activeCursorID;

	// OTHER VARIBLES FOR RENDERING
	int x;
	int y;
	int z;
	int alpha;
	int width;
	int height;

public:
	// INLINED ACCESSOR METHODS
	unsigned int		getImageID(int index)	{ return imageIDs->at(index);	}
	unsigned int		getActiveCursorID()		{ return activeCursorID;		}
	int					getX()					{ return x;						}
	int					getY()					{ return y;						}
	int					getZ()					{ return z;						}
	int					getAlpha()				{ return alpha;					}
	int					getWidth()				{ return width;					}
	int					getHeight()				{ return height;				}
	unsigned int		getNumCursorIDs()		{ return imageIDs->size();		}

	// INLINED MUTATOR METHODS
	void setActiveCursorID(unsigned int initActiveCursorID)
	{ activeCursorID = initActiveCursorID;	}
	void setX(int initX)
	{ x = initX;							}
	void setY(int initY)
	{ y = initY;							}
	void setZ(int initZ)
	{ z = initZ;							}
	void setAlpha(int initAlpha)
	{ alpha = initAlpha;					}

	// METHODS DEFINED IN Cursor.cpp
	Cursor();
	~Cursor();
	void addRenderItemToRenderList(RenderList *renderList);
	void initCursor(	vector<unsigned int> *initImageIDs, 
						unsigned int initActiveCursorID, 
						int initX, int initY, int initZ, 
						int initAlpha, 
						int initWidth, int initHeight);
};