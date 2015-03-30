/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsInput.cpp

	See WindowsInput.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gui\Button.h"
#include "sssf\gui\Cursor.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\input\KeyEventHandler.h"
#include "sssf\os\GameOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsOS.h"

/*
	WindowsInput - Default constructor, it will initialize the input state variables.
*/
WindowsInput::WindowsInput()
{
	mousePoint = new POINT();
	mousePoint->x = 0;
	mousePoint->y = 0;
}

/*
	~WindowsInput - Destructor, it will destroy the mousePoint pointer.
*/
WindowsInput::~WindowsInput()
{
	delete mousePoint;
}

/*
	processInput - This method updates first gets all input from Windows
	for the mouse and the keyboard. It then asks the event handlers
	to respond to the input.
*/
void WindowsInput::processInput(Game *game)
{
	WindowsOS *os = (WindowsOS*)game->getOS();
	WINDOWINFO wi = os->getWindowInfo();
	updateCursorPosition(game, wi, game->getGUI()->getCursor());
	updateInputState();
	respondToKeyboardInput(game);
	respondToMouseInput(game);
}

/*
	respondToMouseInput - This method sends the updated cursor position
	to the GameGUI so that it can update the Button and Cursor states.
	It then checks to see if the left mouse button is pressed, and if
	so, it asks the gui to check to see if it needs to fire an event.
	This should be called once per frame, after input is retrieved.
*/
void WindowsInput::respondToMouseInput(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameStateManager *gsm = game->getGSM();
	gui->updateGUIState(mousePoint->x, mousePoint->y, gsm->getCurrentGameState());
	
	if ( (GetAsyncKeyState(VK_LBUTTON) & 0X8000)
		&& (inputState[VK_LBUTTON].isFirstPress))
	{
		gui->checkCurrentScreenForAction(game);
	}
}

/*
	updateCursorPosition - This method asks Windows for the position
	of the cursor in screen coordinates. The cursor position is fixed
	to account for windows borders. The values are recorded for use
	and the cursor is updated.
*/
void WindowsInput::updateCursorPosition(Game *game, WINDOWINFO wi, Cursor *cursor)
{
	GetCursorPos(mousePoint);
	GameGraphics *graphics = game->getGraphics();

	// Fix the mouse location
	mousePoint->x = mousePoint->x - wi.rcWindow.left - wi.rcClient.left;
	mousePoint->y = mousePoint->y - wi.rcWindow.top - wi.rcClient.top;
	if (mousePoint->x < 0)
	{
		mousePoint->x = 0;
	}
	if (mousePoint->x >= graphics->getScreenWidth())
	{
		mousePoint->x = graphics->getScreenWidth() - 1;
	}
	if (mousePoint->y < 0)
	{
		mousePoint->y = 0;
	}
	if (mousePoint->y >= graphics->getScreenHeight())
	{
		mousePoint->y = graphics->getScreenHeight() - 1;
	}

	cursor->setX(mousePoint->x);
	cursor->setY(mousePoint->y);
}

/*
	updateInputState - This method checks all keys and updates
	their states. This should be called once per frame.
*/
void WindowsInput::updateInputState()
{
	// RESET isPressed FOR ALL KEYS
	for (int i = 0; i < 256; i++)
		inputState[i].isPressed = false;

	// FILL IN isPressed FOR THOSE PRESSED
	for (int j = 0; j < 256; j++)
	{
		if (GetAsyncKeyState(j) & 0X8000)
			inputState[j].isPressed = true;
	}

	// UPDATE wasHeldDown & isFirstPress
	for (int k = 0; k < 256; k++) 
	{
		if (inputState[k].isPressed) 
		{
			if (inputState[k].wasHeldDown)
				inputState[k].isFirstPress = false;
			else 
			{
				inputState[k].wasHeldDown = true;
				inputState[k].isFirstPress = true;
			}
		}
		else 
		{
			inputState[k].wasHeldDown = false;
			inputState[k].isFirstPress = false;
		}
	}
}

/*
	shutdown - we are simply using Windows for getting input, but, if we were using
	another library, like DirectInput for getting game controller data, we may have
	to tie up some loose ends here.
*/
void WindowsInput::shutdown()
{
	// IF WE WERE USING ANOTHER LIBRARY, WE MAY NEED TO
	// SHUT THINGS DOWN
}