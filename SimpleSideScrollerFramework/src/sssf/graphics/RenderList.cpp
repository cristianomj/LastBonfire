/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	RenderList.cpp

	See RenderList.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\graphics\RenderList.h"

/*
	RenderList - Default Constructor, it constructs the list but
	does not fill it. Filling the list should be done via addRenderItem.
*/
RenderList::RenderList(int initMaxItems)
{
	maxItems = initMaxItems;
	renderList = new RenderItem[maxItems];
	iteratorIndex = 0;
	size = 0;
}

/*
	~RenderList - Destructor, this method cleans up the renderList.
*/
RenderList::~RenderList()
{
	delete renderList;
}

/*
	addRenderItem - This method adds a constructed RenderItem pointer
	to the render list. The result should be that that item would be
	rendered in the next rendering. Note that once rendered, the list
	should be cleared because next frame we'll have different stuff
	to render.
*/
void RenderList::addRenderItem(	unsigned int id,
								int x,
								int y,
								int z,
								int a,
								int width,
								int height)
{
	if (size < maxItems)
	{
		renderList[size].id = id;
		renderList[size].x = x;
		renderList[size].y = y;
		renderList[size].z = z;
		renderList[size].a = a;
		renderList[size].width = width;
		renderList[size].height = height;
		size++;
	}
}

/*
	clear - This method clears out the render list, deleting all the
	RenderItems inside. This should be called after each time a RenderList
	is used for rendering. If it is not called, the render list will get
	huge, and old stuff that has since been updated or moved, or are out
	of view will still be rendered.
*/
void RenderList::clear()
{
	size = 0;
	iteratorIndex = 0;
}

/*
	hasNext - This method helps control the internal iterator for going
	through the list. When it comes time to render, we'll want the elements
	one at a time, in order. This method can check to see if there are any
	more elements left in the iterator to retrieve. If more elements exist,
	true is found, else false is returned. This works just like a traditional
	Iterator.
*/
bool RenderList::hasNext()
{
	return (iteratorIndex < size);
}

/*
	next - This method gets the next element using the internal iterator.
	One should call hasNext before calling this method to check if there
	even are any more elements. If there are no more elements for the
	iterator to retrieve, this method returns NULL.
*/
RenderItem RenderList::next()
{
	if (hasNext())
	{
		RenderItem itemToReturn = renderList[iteratorIndex];
		iteratorIndex++;
		return itemToReturn;
	}
}

/*
	resetIterator - This method resets the iterator back to the
	beginning of the list. It should be called each frame before
	rendering the list.
*/
void RenderList::resetIterator()
{
	iteratorIndex = 0;
}
