
/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsOS.cpp

	See WindowsOS.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\os\GameOS.h"
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\timer\GameTimer.h"

// WE WILL USE blankWindowsCursor TO HIDE THE WINDOWS
// CURSOR. WE ARE DOING THIS BECAUSE WE DON'T LIKE THE
// WAY IT BEHAVES SOMETIMES AND WE WANT OUR OWN EG
// CURSOR THAT WE CAN CHANGE WHENEVER WE WANT AND MAKE
// IT LOOK HOWEVER WE WANT. WE WILL DRAW OUR CURSOR
// IN FUTURE EXAMPLES USING DirectX
static HCURSOR blankWindowsCursor;
static Game *game;

// THIS IS A FORWARD DECLARATION OF OUR WINDOWS MESSAGE
// HANDLING METHOD. IT IS NOT A CLASS METHOD, IT IS
// static, WHICH IS NECESSARY FOR THIS METHOD
LRESULT static CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*
	GameWindow - This constructor creates a EG Windows
	window. All drawing in future examples will use such
	a window, inside of which we will do drawing using
	DirectX. After making the window it displays it.
*/
WindowsOS::WindowsOS(	HINSTANCE hInstance,
						int nCmdShow,
						bool isFullscreen,
						wstring initApplicationName,
						int screenWidth,
						int screenHeight,
						Game *initGame)
{
	game = initGame;

	// THIS WILL BE THE TITLE FOR THE WINDOW
	applicationName = initApplicationName;

	// WINDOWS CAN EITHER BE IN WINDOWED MODE, MEANING
	// THE WINDOW PLAYS NICELY WITH OTHER APPLICATIONS
	// AND CAN BE RESIZED, MINIMIZED, MAXIMIZED, ETC.,
	// OR IT CAN BE IN FULLSCREEN MODE, IN WHICH CASE
	// IT LIKES TO HOG RESOURCES AND CANNOT BE RESIZED.
	// GAMES IN FULL SCREEN MODE GET MORE OF THE SYSTEM
	// RESOURCES, BUT CAN ONLY SWITCH TO ANOTHER
	// APPLICATION VIA EITHER ALT-TAB, OR BY CLOSING
	// THE APPLICATION VIA SOME GAME CONTROL (i.e. BUTTON).
	long initStyle;
	if (isFullscreen)
		initStyle = fullscreenStyle;
	else
		initStyle = windowedStyle;
	
	// WE'LL EGIZE THIS STRUCTURE TO CREATE OUR
	// OWN WINDOW CLASS
    WNDCLASSEX wc;

	// EMPTY THE wc STRUCTURE
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// FILL IN THE wc VARIABLES TO CREATE
	// OUR OWN WINDOW TYPE
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;

	// MESSAGE RELAYING METHOD, WE DEFINE OUR OWN
	// WinProc BELOW. NOTE THAT THIS METHOD DOESN'T
	// HAVE TO BE NAMED WinProc, BUT THAT IS THE
	// COMMON PRACTICE
	wc.lpfnWndProc = (WNDPROC)WinProc;

	// APPLICATION HANDLE
    wc.hInstance = hInstance;

	// WE'LL DEAL WITH THE CURSOR ELSEWHERE
	wc.hCursor = NULL;

//	wc.hIcon = ??
//	wc.hIconSm = ??

	// NAME DOESN'T MATTER AS LONG AS WE USE THE SAME
	// NAME WHEN WE CALL CreateWindowEx
    wc.lpszClassName = L"GameWindowClass";

	// REGISTER OUR NEW wc WINDOW TYPE
    RegisterClassEx(&wc);

	// USE THE CreateWindowEx FACTORY METHOD TO CREATE
	// AN INSTANCE OF OUR OWN EG WINDOW TYPE. THIS
	// METHOD RETURNS A HANDLE TO OUR WINDOW, WHICH
	// WE WILL NEED TO CREATE OTHER OBJECTS, LIKE A
	// DirectX GRAPHICS DEVICE
    windowHandle = CreateWindowEx(NULL,
		L"GameWindowClass",		// OUR WINDOW CLASS TYPE
		applicationName.c_str(),// WINDOW TITLE
		initStyle,				// FULLSCREEN or WINDOWED?
		0, 0,					// WINDOW LOCATION
		screenWidth,	// WINDOW WIDTH
		screenHeight,	// WINDOW HEIGHT
		NULL,					// NO PARENT WINDOW
		NULL,					// NO MENUS
		hInstance,				// APPLICATION HANDLE
		NULL);					// NO MULTIPLE WINDOWS
	
	// LOAD THE BLANK CURSOR FILE
//	LPCWSTR LcursorName = L"BlankCursor.cur";
//	blankWindowsCursor = LoadCursorFromFile(LcursorName);

	// THIS WILL MAKE THE WINDOWS CURSOR DISAPPEAR
	// REMEMBER, WE'LL DRAW OUR OWN ANOTHER WAY
//	SetCursor(blankWindowsCursor);
//	ShowCursor(TRUE);

    // OPEN UP YOUR WINDOW TO THE SCREEN
    ShowWindow(windowHandle, nCmdShow);

	// GET SOME INFO ABOUT THE WINDOW. wi STORES
	// SOME USEFUL INFO ABOUT THE SCREEN AND THE WINDOW
	GetWindowInfo(windowHandle, &wi);
}

/*
	~WindowsGameWindow - This destructor has no pointers
	to clean up.
*/
WindowsOS::~WindowsOS() 
{
	// WE DON'T HAVE ANY POINTERS TO CLEAN UP
}

/*
	manageWindowsMessages - This method is needed to relay
	events to Windows. We should call this method once
	per frame of the game loop. It simply looks in the
	message queue and sends it off to the window's message 
	handler, which we've setup as our WinProc method.
*/
void WindowsOS::processOSMessages()
{
	MSG msg;

	// CHECK TO SEE IF ANY WINDOWS MESSAGES ARE WAITING
	// IN THE QUEUE. FOR EXAMPLE, DID SOMEONE HIT THE
	// WINDOW'S X TO CLOSE THE APPLICATION?
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// TRANSLATE THE MESSAGE AND SEND IT ON TO OUR
		// ASSIGNED WINDOW MESSAGE HANDLER, WHICH WE
		// SPECIFIED AS WinProc WHEN WE CREATED THE WINDOW
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/*
	WinProc - This is for relaying messages from Windows
	to our program. Note that this is not involved in
	getting mouse positions or keyboard entry for our games.
	We'll request that as needed another way using on
	demand Windows methods. This really for making sure
	that when the application is in Windowed mode, if the
	user clicks the X at the top right of the window, that
	the application really does close. Of course, we might
	want to check with the user first to see if they really
	want to exit, but we are not doing that yet.
*/
LRESULT static CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// WHAT TYPE OF EVENT IS IT?
    switch(message)
	{
		// SOMEONE WANTS TO EXIT THE APPLICATION
		case WM_QUIT:
		case WM_DESTROY:
            {
				// THIS WILL END THE GAME LOOP
				game->shutdown();

				// THIS WILL CLOSE THE WINDOW
                PostQuitMessage(0);
                return 0;
            }

			
		// SOMEONE EITHER MOVED THE WINDOW, MAXIMIZED
		// IT, OR THE CURSOR JUST ENTERED THE WINDOW
		// FROM PREVIOUSLY BEING OUTSIDE. THESE THINGS
		// ARE ONLY A CONCERN IN WINDOWED MODE. BTW,
		// WINDOWS DOESN'S SEEM TO FIRE THE WM_NCMOUSELEAVE
		// EVENT WHEN ENTERING THE NON-CLIENT AREA FROM
		// BELOW THE WINDOW IN THE SOUTH, WHICH MEANS
		// THE WINDOWS CURSOR MIGHT POP UP IN THAT CASE.
		// THE SOLUTION? USE FULLSCREEN MODE IN THE RELEASE
		// VERSION OF YOUR GAME
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_NCMOUSELEAVE:
		case WM_WINDOWPOSCHANGED:
		{
			// WE HAVE TO REDO THE CURSOR
			SetCursor(blankWindowsCursor);
			ShowCursor(TRUE);
		} break;
    }

	// LET WINDOWS TAKE CARE OF ANY MESSAGES 
	// WE DON'T CARE ABOUT
    return DefWindowProc (hWnd, message, wParam, lParam);
}

/*
	shutdown - This will destroy the Windows window.
*/
void WindowsOS::shutdown()
{
	PostMessage(windowHandle, WM_DESTROY, 0, 0);
}