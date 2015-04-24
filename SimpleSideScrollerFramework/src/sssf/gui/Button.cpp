/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Button.cpp

	See Button.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gui\Button.h"
#include "sssf\input\ButtonEventHandler.h"
#include "sssf\input\GameInput.h"

/*
	Button - Default Constructor, it sets all int instance
	variables to -1 and all object pointers to NULL.
*/
Button::Button()
{
	normalTextureID = 0;
	mouseOverTextureID = 0;
	x = 0;
	y = 0;
	z = 0;
	width = 0;
	height = 0;
	mouseOver = false;
	eventHandler = NULL;
}

/*
	~Button - Destructor - this cleans up the command pointer, but
	not the event handler because it may be shared.
*/
Button::~Button()	
{
	// WE ARE NOT DELETING THE EVENT HANDLER BECAUSE IT
	// MAY BE SHARED BY MULTIPLE OBJECTS. WHEN THE APPLICATION
	// CLOSES IT SHOULD BE DELETED BY THE GAME
}

/* 	
	fireEvent - The Button has been clicked, tell the
	eventHandler to respond.
*/
void Button::fireEvent(Game *game)
{
	if (eventHandler != NULL)
		eventHandler->handleButtonEvents(game, command);
}

/*
	initButton - This methods initializes all of the 
	instance variables at once except for the event
	handler, which must be set separately.
*/
void Button::initButton(int initNormalTextureID,
				int initMouseOverTextureID,
				int initX,
				int initY,
				int initZ,
				int initAlpha,
				int initWidth,
				int initHeight,
				bool initMouseOver,
				wstring initCommand)
{
	normalTextureID = initNormalTextureID;
	mouseOverTextureID = initMouseOverTextureID;
	x = initX;
	y = initY;
	z = initZ;
	alpha = initAlpha;
	width = initWidth;
	height = initHeight;
	mouseOver = initMouseOver;
	command = initCommand;
}

/*
	updateMouseOver - This methods checks to see if the
	provided mousePoint argument is over the button. If
	it is, it sets the mouseOver instance variable to true
	and returns true. Otherwise, it sets the mouseOver
	instance variable to false and returns false.

	Note that this should only be called once per frame. If
	needed more than once per frame for this button, just
	call it once, and then call isMouseOver thereafter.
	isMouseOver is a simple accessor method that returns
	the instance variable set by this method.
*/
void Button::updateMouseOver(long mouseX, long mouseY)
{
	// IS THE CURSOR OVER THIS BUTTON?
	if ((mouseX >= x)  &&
		(mouseX <= (x + width)) &&
		(mouseY >= y)   &&
		(mouseY <= (y + height)) )
	{
		// SETTING THIS WILL ENSURE WE USE THE PROPER IMAGE FOR
		// RENDERING, IT WILL ALSO ENSURE AN EVENT IS FIRED IF
		// THE MOUSE BUTTON IS CLICKED
		mouseOver = true;
	}
	else
	{
		// THIS WILL ENSURE THE NORMAL IMAGE IS RENDERED
		mouseOver = false;
	}
}