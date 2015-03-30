/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	ScreenGUI.cpp

	See ScreenGUI.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\gui\Button.h"
#include "sssf\gui\ScreenGUI.h"

/*
	ScreenGUI - Default Constructor, it initializes the data structures
	for storing the GUI Buttons and Overlay Images. The screen name must
	be set separately.
*/
ScreenGUI::ScreenGUI()
{
	buttons = new list<Button*>();
	overlayImages = new list<OverlayImage*>();
	screenName = NULL;
}

/*
	~ScreenGUI - This destructor cleans up all the Buttons, OverlayImages,
	and the screen name instance variable pointers, and so their related
	memory.
*/
ScreenGUI::~ScreenGUI()
{
	// TODO - DELETE EACH INDIVIDUAL BUTTON AND OVERLAY IMAGE
	delete buttons;
	delete overlayImages;
	delete screenName;
}

/*
	addButton - This method adds a contructed Button to this GUI screen.
*/
void ScreenGUI::addButton(Button *buttonToAdd)
{
	buttons->push_back(buttonToAdd);
}

/*
	addOverlayImage - This method adds a constructured OverlayImage
	to this GUI screen.
*/
void ScreenGUI::addOverlayImage(OverlayImage *imageToAdd)
{
	overlayImages->push_back(imageToAdd);
}

/*
	addRenderItemsToRenderList - This method should be called once
	per frame on this screen if it is currently active. It goes
	through all the GUI elements, the buttons and the overlay images,
	and for each it generates a RenderItem and adds it to the list.
*/
void ScreenGUI::addRenderItemsToRenderList(RenderList *renderList)
{
	// FIRST ADD THE OVERLAY IMAGES TO THE RENDER LIST
	// FOR THIS WE'LL USE AN ITERATOR
	list<OverlayImage*>::iterator imageIterator;
	imageIterator = overlayImages->begin();
	while (imageIterator != overlayImages->end())
	{
		// GET THE OverlayImage WE WANT TO ADD
		// FROM THE ITERATOR
		OverlayImage *image = (*imageIterator);

		// ADD IT TO THE LIST
		renderList->addRenderItem(	image->imageID,
									image->x,
									image->y,
									image->z,
									image->alpha,
									image->width,
									image->height	);

		// ADVANCE THE ITERATOR
		imageIterator++;
	}

	// NOW ADD THE BUTTONS TO THE RENDER LIST
	int imageId;

	// WE'LL NEED A NEW ITERATOR
	list<Button*>::iterator buttonIterator;
	buttonIterator = buttons->begin();
	list<Button*>::iterator end = buttons->end();
	while(buttonIterator != end)
	{
		// GET THE Button WE WANT TO ADD
		// FROM THE ITERATOR
		Button *buttonToRender = (*buttonIterator);

		// DETERMINE WHICH Button IMAGE TO USE
		if (buttonToRender->isMouseOver())
			imageId = buttonToRender->getMouseOverTextureID();
		else
			imageId = buttonToRender->getNormalTextureID();

		// ADD IT TO THE LIST
		renderList->addRenderItem(	imageId,
									buttonToRender->getX(),
									buttonToRender->getY(),
									buttonToRender->getZ(),
									buttonToRender->getAlpha(),
									buttonToRender->getWidth(),
									buttonToRender->getHeight());

		// ADVANCE THE ITERATOR
		buttonIterator++;
	}
}

/*
	fireButtonCommand - This method should be called when a mouse button is
	pressed. It will test to see if any button events should be fired. This
	is done by going through the buttons and seeing if the cursor is currently
	over any of them. If the cursor is over a button, the registered button
	event handler is fired, and the button itself will send it the appropriate
	action command.
*/
bool ScreenGUI::fireButtonCommand(Game *game)
{
	bool found = false;
	Button *buttonToTest = NULL;
	list<Button*>::iterator buttonIterator;

	// IF THERE ARE NO Buttons, LET'S AVOID A NULL POINTER EXCEPTION
	if (buttons != NULL)
	{
		// START THE ITERATOR AT THE BEGINNING
		buttonIterator = buttons->begin();

		// GO THROUGH ALL THE BUTTONS IF NECESSARY
		while (!found && (buttonIterator != buttons->end()))
		{
			// GET THE CURRENT BUTTON FROM THE ITERATOR
			buttonToTest = (*buttonIterator);

			// IF THE CURSOR IS OVER THE BUTTON, WE'VE FOUND THE BUTTON THAT WAS CLICKED
			if (buttonToTest->isMouseOver())
			{
				// THIS WILL END THE LOOP LOOKING FOR THE BUTTON
				found = true;
			}
			else
				// ADVANCE THE ITERATOR TO LOOK AT THE NEXT ONE
				buttonIterator++;
		}
	}

	// IF WE FOUND THE CURSOR WAS OVER A BUTTON, FIRE THE EVENT
	if (found)
	{
		// THIS BUTTON KNOWS ITS HANDLER AND ITS COMMAND
		// SO IT WILL TAKE CARE OF IT
		buttonToTest->fireEvent(game);
		return true;
	}
	else
		// NO EVENTS FIRED, THE CURSOR WAS NOT OVER A BUTTON
		// WHEN IT WAS PRESSED
		return false;
}

/*
	registerButtonEventHandler - This method lets all the buttons in this
	ScreenGUI know the object that will handle their events, should they
	be clicked. This method only needs to be callled once for each screen
	gui, when it is being loaded.
*/
void ScreenGUI::registerButtonEventHandler(ButtonEventHandler *eventHandler)
{
	list<Button*>::iterator buttonIterator;

	// WE WANT TO AVOID A NULL POINTER EXCEPTION SHOULD
	// THERE BE NO BUTTONS IN THIS GUI SCREEN
	if (buttons != NULL)
	{
		// START THE ITERATOR AT THE FIRST BUTTON
		buttonIterator = buttons->begin();
		Button *buttonToSet;

		while (buttonIterator != buttons->end())
		{
			// GET THE CURRENT BUTTON FROM THE ITERATOR
			buttonToSet = (*buttonIterator);

			// LET THE BUTTON HAVE THE EVENT HANDLER
			buttonToSet->setEventHandler(eventHandler);

			// ADVANCE THE ITERATOR
			buttonIterator++;
		}
	}
}

/*
	updateAllButtons - This method is given mousePoint, which
	is the current x,y location of the cursor, and it goes
	through all of the buttons in this screen, determining for
	each if the cursor is currently over it. A button will 
	record whether the cursor is over it or not, such that it will
	know which image to render (mouse over or normal). In addition,
	if the mouse button is clicked, all buttons will know if the 
	mouse is currently over them or not.

	This method should be called once per frame for the currently
	visible ScreenGUI such that the buttons are rendered properly
	and so that button clicks fire events.
*/
void ScreenGUI::updateAllButtons(long mouseX, long mouseY)
{
	list<Button*>::iterator buttonIterator;

	// WE DON'T WANT NULL POINTER EXCEPTIONS, THERE
	// MIGHT NOT BE ANY BUTTONS
	if (buttons != NULL)
	{
		// START THE ITERATOR
		buttonIterator = buttons->begin();
		Button *buttonToUpdate;
		while (buttonIterator != buttons->end())
		{
			// GET THE CURRENT BUTTON FROM THE ITERATOR
			buttonToUpdate = (*buttonIterator);

			// UPDATE ALL BUTTONS IN THIS SCREEN SO THEY
			// KNOW IF THE CURSOR IS CURRENTLY OVER THEM
			// OR NOT
			buttonToUpdate->updateMouseOver(mouseX, mouseY);

			// ADVANCE THE ITERATOR
			buttonIterator++;
		}
	}
}
