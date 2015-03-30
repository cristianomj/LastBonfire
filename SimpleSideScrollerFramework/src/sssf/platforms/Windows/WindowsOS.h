#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\os\GameOS.h"

/*
	WindowsOS - This method provides a Windows platform
	implementation of OS message - to - game application handling.
	This ensures that our program can handle all input accordingly.
*/

class Game;

// THESE CONSTANTS ARE USED TO CONSTRUCT THE WINDOW
// DEPENDING ON WHETHER WE WANT IT IN WINDOWED MODE
// OR FULLSCREEN MODE
const long windowedStyle = WS_OVERLAPPEDWINDOW;
const long fullscreenStyle = WS_POPUP|WS_SYSMENU|WS_VISIBLE;

class WindowsOS : public GameOS
{
	// 3 VARIABLES FROM Game
	bool	fullscreen;

	// DATA USED TO INIT GAME DEVICES
	HINSTANCE hInstance;
	int nCmdShow;

	// windowHandle IS A POINTER TO THE APPLICATION
	// IT IS NEEDED BY DIRECTX TO CONSTRUCT CERTAIN
	// DEVICES, LIKE A GRAPHICS DEVICE
	HWND		windowHandle;
	
	// wi HAS INFORMATION ABOUT THE WINDOW, LIKE THE
	// DIMENSIONS, AMONG OTHER THINGS
	WINDOWINFO	wi;

	// applicationName STORES THE TEXT IN THE TITLE
	// OF THE WINDOW
	wstring applicationName;

public:
	// INLINED ACCESSOR METHODS
	bool isFullscreen() { return fullscreen;}

	HWND		getWindowHandle()	{ return windowHandle;	}
	WINDOWINFO	getWindowInfo()		{ return wi;			}

	// INLINED MUTATOR METHOD
	void setApplicationName(wstring initApplicationName)
	{
		applicationName = initApplicationName;
		SetWindowText(windowHandle, applicationName.c_str());
	}

	// METHODS DEFINED IN WindowsOS.cpp
	WindowsOS(	HINSTANCE hInstance,
				int nCmdShow,
				bool isFullscreen,
				wstring initApplicationName,
				int screenWidth,
				int screenHeight,
				Game *initGame);
	~WindowsOS();
	WPARAM manageWindowsMessages();
	void processOSMessages();
	void shutdown();
};