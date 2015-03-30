/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Viewport.cpp

	See Viewport.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gui\Viewport.h"

/*
	Viewport - This default constructor simply sets all integer
	values to -1, which means they have not yet been assigned
	valid values.
*/
Viewport::Viewport()
{
	// NORMAL VIEW TO START
	viewportToggled = false;

	// DEFAULT SCROLL SPEED
	scrollSpeedX = 10;
	scrollSpeedY = 10;

	// NO DEFAULTS FOR THESE
	viewportX = 0;
	viewportY = 0;
	viewportWidth = 0;
	viewportHeight = 0;
	viewportOffsetX = 0;
	viewportOffsetY = 0;
}

/*
	~Viewport - This class has no pointer instance variables
	so this destructor has nothing to clean up.
*/
Viewport::~Viewport()
{

}

bool Viewport::areViewportCoordinatesInViewport(int x,
												int y,
												int width,
												int height)
{
	// IS IT OFF-SCREEN TO THE LEFT OF THE VIEWPORT?
	if		((x + width) <= 0)
		return false;

	// IS IT OFF-SCREEN ABOVE THE VIEWPORT?
	else if	((y + height) <= 0)
		return false;

	// IS IT OFF-SCREEN TO THE RIGHT OF THE VIEWPORT?
	else if (x >= viewportWidth)
		return false;

	// IS IT OFF-SCREEN BELOW THE VIEWPORT?
	else if (y >= viewportHeight)
		return false;

	// IT MUST BE AT LEAST PARTIALLY IN THE VIEWPORT
	else
		return true;
}


/*
	isInViewport - This method tests to see if a
	rectangular region (i.e. a texture) overlaps the
	viewport. true is returned if it overlaps, false
	otherwise. This could be used for checking to
	see if a sprite is in the viewport, and so 
	deciding if it should be added to the render list.
	This is done by checking to see if the texture
	is to the left of the left-most edge of the viewport,
	to the right of the right-most edge, above the top-
	most edge, or below the bottom-most edge. In any
	of these cases, false is returned because the
	rectangle doesn't overlap the viewport. If none
	of these cases are true, the rectangle must be
	at least partially inside the viewport.
*/
bool Viewport::areWorldCoordinatesInViewport(	float x,
												float y,
												int width,
												int height)
{
	// IS IT OFF-SCREEN TO THE LEFT OF THE VIEWPORT?
	if		((x + width) <= viewportX)
		return false;

	// IS IT OFF-SCREEN ABOVE THE VIEWPORT?
	else if	((y + height) <= viewportY)
		return false;

	// IS IT OFF-SCREEN TO THE RIGHT OF THE VIEWPORT?
	else if (x >= (viewportX + viewportWidth))
		return false;

	// IS IT OFF-SCREEN BELOW THE VIEWPORT?
	else if (y >= (viewportY + viewportHeight))
		return false;

	// IT MUST BE AT LEAST PARTIALLY IN THE VIEWPORT
	else
		return true;
}

/*
	moveViewport - This method is for moving the
	viewport, which is the region of the world seen
	by the player. This would typically be tied to 
	player movement or some scripted sequence. This
	method prevents moving the viewport off the edge
	of the world by clamping at the world edges.
*/
void Viewport::moveViewport(int incX, 
							int incY,
							int worldWidth, 
							int worldHeight)
{
	// MOVE THE VIEWPORT IN X AXIS ACCORDING
	// TO PROVIDED INCREMENT.
	viewportX += incX;

	// DON'T LET IT GO OFF THE LEFT-MOST EDGE
	// OF THE GAME WORLD. CLAMP INSTEAD.
	if (viewportX < 0)				
		viewportX = 0;

	// DON'T LET IT GO OFF THE RIGHT-MOST EDGE
	// OF THE GAME WORLD. CLAMP INSTEAD.
	else if (viewportX >= (worldWidth - viewportWidth))
		viewportX = worldWidth - viewportWidth - 1;
	
	// MOVE THE VIEWPORT IN Y AXIS ACCORDING
	// TO PROVIDED INCREMENT.
	viewportY += incY;

	// DON'T LET IT GO OFF THE TOP-MOST EDGE
	// OF THE GAME WORLD. CLAMP INSTEAD.
	if (viewportY < 0)				
		viewportY = 0;

	// DON'T LET IT GO OFF THE BOTTOM-MOST EDGE
	// OF THE GAME WORLD. CLAMP INSTEAD.
	else if (viewportY >= worldHeight - viewportHeight)
		viewportY = worldHeight - viewportHeight - 1;
}


bool Viewport::areScreenCoordinatesInViewport(int x, int y)
{
	if (x < viewportOffsetX) return false;
	if (y < viewportOffsetY) return false;
	if (x > (viewportOffsetX + viewportWidth)) return false;
	if (y > (viewportOffsetY + viewportHeight)) return false;
	return true;
}

void Viewport::toggleDebugView()
{
	if (viewportToggled)
	{
		viewportOffsetY = normalOffsetY;
	}
	else
	{
		viewportOffsetY = toggleOffsetY;
	}
	viewportToggled = !viewportToggled;
}