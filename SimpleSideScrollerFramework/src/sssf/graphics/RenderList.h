/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	RenderList.h

	This class stores the data for rendering a list of
	textures each frame. While the game is running, each
	frame, lists of RenderItems may be generated for all
	visible textures. That list is then traversed and
	used for rendering and then destroyed.

	RenderItems should be placed into the list in the
	order with which they should be rendered, background
	items at the start of the list and foreground items
	at the end of the list. Rendering should be done in 
	the order of game world	and then game gui. 

	RenderItems have z values, so optionally we could
	sort the items in the list before rendering in order
	to ensure correct back to front rendering.

	Each RenderItem also has an alpha value such that each
	item can have its own custom transparency.
	
	The game world should be rendered in order of the
	game background and then game objects and characters.
	The game gui should be rendered in order of the gui
	overlay images, then gui buttons, and then the cursor.

*/

#pragma once
#include "sssf_VS\stdafx.h"

/*
	RenderItem - This structure represents one texture to be rendered
	each frame. Each frame we should build 2 lists of these. One for the 
	game world (if game is active), and one for the gui. Note that
	this struct can compare 2 RenderItems to each other for the purpose
	of sorting via z value.
*/
struct RenderItem : public std::binary_function<RenderItem, RenderItem, bool>
{
	unsigned int id;
	int x;
	int y;
	int z;
	int a;
	int width;
	int height;

	// WE HAVE DEFINED THIS COMPARISON SUCH THAT OUR LIST CAN BE
	// SORTED USING THE sort METHOD FROM THE STL'S list CLASS.
	// THE () OPERATOR WILL ALLOW US TO SORT RenderItems IN THE
	// LIST BY THEIR Z VALUES
	bool operator()(RenderItem renderItemOne, RenderItem renderItemTwo) const
	{
		if (renderItemOne.z < renderItemTwo.z)
			return true;
		else
			return false;
	};
};

class RenderList
{
private:
	// WE ARE GOING TO USE PRE-MADE ARRAYS OF RenderItems ACTUALLY,
	// NOT A LIST. THIS ARRAY WILL HAVE A CAPACITY AND WE'LL SIMPLY
	// REUSE THESE CONSTRUCTED OBJECTS FRAME AFTER FRAME
	RenderItem *renderList;
	int size;
	int	maxItems;
	int iteratorIndex;

public:
	// METHODS DEFINED IN RenderList.cpp
	RenderList(int initMaxItems);
	~RenderList();
	void		addRenderItem(	unsigned int id,
								int x,
								int y,
								int z,
								int a,
								int width,
								int height);
	void		clear();
	bool		hasNext();
	RenderItem	next();
	void		resetIterator();
};