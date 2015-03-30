/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DirectXGraphics.cpp

	See DirectXGraphics.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\Viewport.h"
#include "sssf\os\GameOS.h"
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\text\GameText.h"

/*
	DirectXGraphics - Default constructor, it doesn't initialize anything.
	To setup all the DirectX objects call initGraphics after construction.
*/
DirectXGraphics::DirectXGraphics(Game *initGame)
{
	game = initGame;
}

/*
	~DirectXGraphics - Destructor, it destroys all of the DirectX pointers.
	This would be called only when the application closes, unless someone 
	decides to use different rendering technologies during the game.
*/
DirectXGraphics::~DirectXGraphics()
{
	delete d3d;
	delete graphicsDevice;
	delete spriteHandler;
	delete textFont;
}

/*
	containsDisplayMode - This method looks at the vector of display modes that
	were presumably retrieved from the GPU, and tests to see if our desired
	color format and screen resolution are inside. If found, true is returned,
	otherwise false.
*/
bool DirectXGraphics::containsDisplayMode(vector<D3DDISPLAYMODE*> *displayModes,
										  D3DFORMAT testColorFormat,
										  int testScreenWidth,
										  int testScreenHeight)
{
	vector<D3DDISPLAYMODE*>::iterator iterator;

	// START WITH THE FIRST ELEMENT
	iterator = displayModes->begin();

	// LOOK THROUGH THE WHOLE VECTOR
	while (iterator != displayModes->end())
	{
		// GET THE CURRENT MODE
		D3DDISPLAYMODE *testMode = (*iterator);

		// IS IT THE ONE WE'RE LOOKING FOR?
		if ((testMode->Format == testColorFormat)
			&& (testMode->Width == testScreenWidth)
			&& (testMode->Height == testScreenHeight))
			return true;

		// GO ONTO THE NEXT ONE
		iterator++;
	}
	return false;
}

/*
	createDirectXDeviceAndSpriteHandler - THIS METHOD CREATES OUR GPU AND
	SPRITE HANDLER (used for batch rendering textures) USING THE COLOR
	FORMAT AND SCREEN RESOLUTION OF OUR CHOICE.
*/
HRESULT DirectXGraphics::createDirectXDeviceAndSpriteHandler()
{
	HRESULT result;
	GameText *text = game->getText();

	// CREATE OUR GPU
    result = d3d->CreateDevice(D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						presentParameters.hDeviceWindow,
						D3DCREATE_HARDWARE_VERTEXPROCESSING,
						&presentParameters,
						&graphicsDevice);
	
	// IF GPU CREATION WAS SUCCESSFUL
	if (SUCCEEDED(result))
	{
		text->writeDebugOutput("SUCCEEDED");

		text->writeDebugOutput("\nD3DXCreateSprite(): ");

		// CREATE OUR SPRITE HANDLER
	    result = D3DXCreateSprite(graphicsDevice, &spriteHandler);
		if (SUCCEEDED(result))
		{
			text->writeDebugOutput("SUCCEEDED");
		}
		else
			text->writeDebugOutput("FAILED");

		this->initTextFont(20);
	}
	else
		text->writeDebugOutput("FAILED");

	return result;
}

/*
	createTextureManager - This method constructs a technology-specific
	TextureManager. Since this DirectXGraphics class uses the DirectX
	library, this method creates a DirectXTextureManager.
*/
TextureManager* DirectXGraphics::createTextureManager()
{
	TextureManager *textureManager = (TextureManager*)(new DirectXTextureManager());
	textureManager->setGraphics(this);
	return textureManager;
}

/*
	endDirectXFrameRendering - This method should be called after rendering a frame
	so that we can display what we've drawn on the GPU onto the monitor. It also
	releases a lock on the GPU so other threads may use it.
*/
void DirectXGraphics::endDirectXFrameRendering()
{
	// ALL DONE DRAWING ONTO THE GPU FOR THIS FRAME
    if (FAILED(graphicsDevice->EndScene()))
		game->getText()->writeDebugOutput("\ngraphicsDevice->EndScene(): FAILED");

	// PUT WHAT WE JUST RENDERED ONTO THE SCREEN
	if (FAILED(graphicsDevice->Present(NULL, NULL, NULL, NULL)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Present(): FAILED");
}

/*
	findAlternativeDisplayMode - If the player's GPU doesn't have the
	display mode we want this method can pick a new one. It does so
	by first seeing if there is another display mode with the resolution
	we want but a different color model. If found, we'll use it. If not it
	simply picks the largest one it can find. This method uses call-by-reference
	to set the formatToSet, screenWidthToSet, & screenHeightToSet parameters
	using the chosen display mode parameters.
*/
void DirectXGraphics::findAlternativeDisplayMode(	vector<D3DDISPLAYMODE*> *displayModes,
													D3DFORMAT &formatToSet)
{
	// FIRST FIND ONE WITH THE PREFERRED SCREEN
	// DIMENSIONS, DEFAULT_SCREEN_HEIGHT &
	// DEFAULT_SCREEN_WIDTH, SINCE CHANGING A GUI TO
	// FIT DIFFERENT RESOLUTIONS IS PAINFUL
	bool defaultScreenSizeFound = false;
	vector<D3DDISPLAYMODE*>::iterator iterator;
	iterator = displayModes->begin();
	while (iterator != displayModes->end())
	{
		D3DDISPLAYMODE *testMode = (*iterator);
		if ((testMode->Width == screenWidth)
			&& (testMode->Height == screenHeight))
		{
			if (testMode->Format == DEFAULT_COLOR_FORMAT)
			{
				formatToSet = DEFAULT_COLOR_FORMAT;
				return;
			}
			else
			{
				defaultScreenSizeFound = true;
				formatToSet = testMode->Format;
			}
		}
		iterator++;
	}

	if (defaultScreenSizeFound)
		return;

	// NONE WERE FOUND IN THE SCREEN SIZE WE WANT, SO
	// NOW JUST FIND THE LARGEST RESOLUTION AVAILABLE
	int totalPixels = 0;
	int maxPixels = 0;
	iterator = displayModes->begin();
	while (iterator != displayModes->end())
	{
		D3DDISPLAYMODE *testMode = (*iterator);
		totalPixels = testMode->Width * testMode->Height;
		if (totalPixels > maxPixels)
		{
			formatToSet = testMode->Format;
			screenWidth = testMode->Width;
			screenHeight = testMode->Height;
			maxPixels = totalPixels;
		}
		iterator++;
	}
}

/*
	getDirectXDisplayModes - This method queries the GPU and gets a vector
	of all the display modes available, returning this data structure.
*/
vector<D3DDISPLAYMODE*>* DirectXGraphics::getDirectXDisplayModes()
{
	// WE'LL FILL THIS WITH THE DISPLAY MODES WE FIND
	vector<D3DDISPLAYMODE*> *displayOptions = new vector<D3DDISPLAYMODE*>();

	// WE'LL LOOK AT EACH COLOR MODEL, STARTING WITH ONE AT INDEX 1, IF THERE IS ONE
	int adapterCounter = 1;
	D3DFORMAT format;
	while (adapterCounter < 1000)
	{
		format = D3DFORMAT(adapterCounter);

		// HOW MANY MODES HAVE THIS COLOR MODEL?
		int numAdapters = d3d->GetAdapterModeCount(
										D3DADAPTER_DEFAULT,
										format);
	
		// GET ALL FOR THIS COLOR MODEL
		D3DDISPLAYMODE *displayModes = new D3DDISPLAYMODE[numAdapters];
		for (int i = 0; i < numAdapters; i++)
		{
			d3d->EnumAdapterModes(	D3DADAPTER_DEFAULT,
									format,
									i,
									&displayModes[i]);

			// PUT THEM INTO OUR VECTOR
			displayOptions->push_back(&displayModes[i]);
		}
		adapterCounter++;
	}
	return displayOptions;
}

/*
	getScreenHeight - This method gets the screen height being used for rendering.
*/
int DirectXGraphics::getScreenHeight()
{
	// ASK THE GRAPHICS CARD
/*	LPDIRECT3DSURFACE9 backbuffer;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DSURFACE_DESC surfaceDescription;
	backbuffer->GetDesc(&surfaceDescription);
	return surfaceDescription.Height;
	*/
	return screenHeight;
}

/*
	getScreenWidth - This method gets the screen width being used for rendering.
*/
int DirectXGraphics::getScreenWidth()
{
	// ASK THE GRAPHICS CARD
/*	LPDIRECT3DSURFACE9 backbuffer;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DSURFACE_DESC surfaceDescription;
	backbuffer->GetDesc(&surfaceDescription);
	return surfaceDescription.Width;
	*/
	return screenWidth;
}

/*
	init - This is the entry point for the application setting up the
	DirectX objects. It will get all available display modes and pick one,
	then use it to make a GPU device. Once this method is called, rendering
	can begin. It only needs to be called once at the start of the application.
	Even if we lose the graphics card (ALT-TAB), we don't have to re-init. We
	would have to re-init if we wished to change from fullscreen mode to 
	windowed mode, or if we want to change the screen resolution or color model.
*/
void DirectXGraphics::initGraphics(GameOS *os, bool isFullscreen)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	// WHAT ARE THE CAPABILITIES OF THE PLAYER'S GPU?
	vector<D3DDISPLAYMODE*> *displayOptions = getDirectXDisplayModes();

	// THESE WILL MAKE UP OUR DESIRED DISPLAY MODE
	D3DFORMAT formatToUse;

	// DOES THE PLAYER HAVE OUR DESIRED FORMAT?
	if (containsDisplayMode(displayOptions,
							DEFAULT_COLOR_FORMAT,
							screenWidth,
							screenHeight))
	{
		// THE GPU HAS OUR DESIRED FORMAT
		formatToUse = DEFAULT_COLOR_FORMAT;
	}
	else
	{
		// THE GPU DOESN'T HAVE OUR DESIRED FORMAT, WE NEED TO PICK ANOTHER ONE
		findAlternativeDisplayMode(	displayOptions,
									formatToUse);
	}

	// WE NEED TO FILL OUT A D3DPRESENT_PARAMETERS STRUCTURE WITH OUR
	// PREFERENCES FOR CREATING OUR GPU DEVICE

	// FIRST MAKE SURE OUR STRUCTURE IS EMPTY
    ZeroMemory(&presentParameters, sizeof(presentParameters));

	// WINDOWED MODE OR FULLSCREEN?
    presentParameters.Windowed = !isFullscreen;

	// DISCARD OLD FRAMES
	presentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;// D3DSWAPEFFECT_DISCARD;

	// THE WINDOW HANDLE
	HWND hWnd;

	// MAKE SURE WE'RE USING WINDOWS
	if (dynamic_cast<WindowsOS*> (os))
	{
		hWnd = ((WindowsOS*)(os))->getWindowHandle();
		presentParameters.hDeviceWindow = hWnd;
	}
	// I SUPPOSE WE COULD DIRECTX ON ANOTHER PLATFORM
	// LIKE XBox OR WINDOWS MOBILE, SO THOSE TESTS
	// AND INITIALIZATION WOULD HAVE TO GO HERE

	// THE DISPLAY MODE WE WILL BE USING
    presentParameters.BackBufferFormat	= formatToUse;
    presentParameters.BackBufferWidth	= screenWidth;
    presentParameters.BackBufferHeight	= screenHeight;

	// OK, NOW WE CAN MAKE OUR GPU & SPRITE HANDLER.
	createDirectXDeviceAndSpriteHandler();
}

/*
	initTextFont - This method will initialize our font object, which
	we need to do all text rendering. It only needs to be done at the
	start of the application unless we want to change the font we
	are using.
*/
void DirectXGraphics::initTextFont(int fontSize)
{
	HRESULT result = D3DXCreateFont(
		graphicsDevice,				// OUR GPU
		fontSize,					// EG FONT SIZE FOR HEIGHT
		0,							// 0 FOR FONT WIDTH, USE DEFAULT ADJUST BASED ON HEIGHT
		FW_BOLD,					// FONT WEIGHT
		0,							// MIP LEVELS
		FALSE,						// ITALICIZED?
		DEFAULT_CHARSET,			// CHARACTER SET
		OUT_DEFAULT_PRECIS,			// RENDERING PRECISION
		DEFAULT_QUALITY,			// RENDERING QUALITY 
		DEFAULT_PITCH | FF_MODERN,	// FONT FAMILY NAME
		TEXT(""),					// FONT FACE NAME
		&textFont );				// THE FONT WE ARE CREATING
}

/*
	reloadGraphics - This method recreates the GPU and sprite handler and
	then reloads all the textures in the current texture managers. This would
	be called after regaining the GPU.
*/
void DirectXGraphics::reloadGraphics()
{
	createDirectXDeviceAndSpriteHandler();
	initTextFont(20);
	guiTextureManager->reloadAllTextures();
	worldTextureManager->reloadAllTextures();
}

/*
	renderDirectXRenderList - This method renders a render list of game
	elements to the screen. It can process render lists for the game
	world or the gui. Note that GUI render lists use screen coordinates
	and so don't have to be offset, but game world lists use world
	coordinates, and so they will need to be offset.
*/
void DirectXGraphics::renderGUIRenderList()
{
	guiRenderList->resetIterator();
	RenderItem itemToRender;
	LPDIRECT3DTEXTURE9 texture;
	RECT *rect = NULL;

	// GO THROUGH EACH ITEM IN THE LIST
	while (guiRenderList->hasNext())
	{
		if (rect != NULL)
			delete rect;
		rect = NULL;
		itemToRender = guiRenderList->next();

		// LET'S GET THE TEXTURE WE WANT TO RENDER
		int id = itemToRender.id;
		texture = ((DirectXTextureManager*)guiTextureManager)->getTexture(id);
		D3DXVECTOR3 position = D3DXVECTOR3(	(FLOAT)(itemToRender.x),
											(FLOAT)(itemToRender.y),
											0);

		// RENDER THE OPAQUE ITEMS
		if (itemToRender.a == 255)
		{
			if (FAILED(spriteHandler->Draw(
					texture, 
					rect,
			        NULL,
					&position,
					DEFAULT_ALPHA_COLOR)))
			{
				game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
			}
		}
			
		// RENDER THE ITEMS WITH EG TRANSPARENCY
		else
		{
			if (itemToRender.a < 0)
				itemToRender.a = 0;
			else if (itemToRender.a > 255)
				itemToRender.a = 255;

			if (FAILED(spriteHandler->Draw(
					texture,
					rect,
					NULL,
					&position,
					D3DCOLOR_ARGB(itemToRender.a, 255, 255, 255))))
			{
				game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
			}
		}
	}

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	guiRenderList->clear();
	if (rect != NULL)
		delete rect;
}

void DirectXGraphics::renderWorldRenderList()
{
	worldRenderList->resetIterator();
	RenderItem itemToRender;
	LPDIRECT3DTEXTURE9 texture;
	RECT *rect = NULL;
	GameGUI *gui = game->getGUI();
	Viewport *viewport = gui->getViewport();

	// GO THROUGH EACH ITEM IN THE LIST
	while (worldRenderList->hasNext())
	{
		if (rect != NULL)
			delete rect;
		rect = NULL;
		itemToRender = worldRenderList->next();
		
		// LET'S GET THE TEXTURE WE WANT TO RENDER
		int id = itemToRender.id;
		if (id >= 0)
		{
			texture = ((DirectXTextureManager*)worldTextureManager)->getTexture(id);
			D3DXVECTOR3 position = D3DXVECTOR3(	(FLOAT)(itemToRender.x),
											(FLOAT)(itemToRender.y),
												0);

			position.x += viewport->getViewportOffsetX();
			position.y += viewport->getViewportOffsetY();

			// ADJUST FOR THE GUI OFFSET
			if ((position.x < viewport->getViewportOffsetX())
				||  (position.y < viewport->getViewportOffsetY()))
			{
				IDirect3DSurface9 *surface;
				UINT level = 0;
				HRESULT result = texture->GetSurfaceLevel(level, &surface);
				D3DSURFACE_DESC surfaceDescription;
				surface->GetDesc(&surfaceDescription);
				rect = new RECT();
				rect->left = 0;
				rect->top = 0;
				rect->right = surfaceDescription.Width;
				rect->bottom = surfaceDescription.Height;
				if (position.x < viewport->getViewportOffsetX())
				{
					int xDiff = viewport->getViewportOffsetX() - (int)position.x;
					rect->left = xDiff;
					position.x += xDiff;
				}
				if (position.y < viewport->getViewportOffsetY())
				{
					int yDiff = viewport->getViewportOffsetY() - (int)position.y;
					rect->top = yDiff;
					position.y += yDiff;
				}	
			}


			// RENDER THE OPAQUE ITEMS
			if (itemToRender.a == 255)
			{
				if (FAILED(spriteHandler->Draw(
					texture, 
					rect,
			        NULL,
					&position,
					DEFAULT_ALPHA_COLOR)))
				{
					game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
				}
			
				// RENDER THE ITEMS WITH CUSTOM TRANSPARENCY
				else
				{
					if (itemToRender.a < 0)
						itemToRender.a = 0;
					else if (itemToRender.a > 255)
						itemToRender.a = 255;

					if (FAILED(spriteHandler->Draw(
						texture,
						rect,
						NULL,
						&position,
						D3DCOLOR_ARGB(itemToRender.a, 255, 255, 255))))
					{
						game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
					}
				}
			}
		}
	}

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	worldRenderList->clear();
	if (rect != NULL)
		delete rect;
}


void DirectXGraphics::renderGame(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	GameText *text = game->getText();

	// CHECK TO SEE IF WE STILL HAVE THE GPU
	HRESULT result = graphicsDevice->TestCooperativeLevel();

	// IF WE HAVE THE GPU, RENDER THE GAME
	if (SUCCEEDED(result)) 
	{
		// NOW PREPARE TO RENDER THE LISTS USING
		// BATCH TEXTURE RENDERING
		startDirectXFrameRendering();	
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// RENDER THE WORLD RENDER LIST
		renderWorldRenderList();
		
		// RENDER THE GUI RENDER LIST
		renderGUIRenderList();

		// RENDER THE TEXT
		renderText(text);

		// WRAP UP RENDERING RESOURCES
		if (FAILED(spriteHandler->End()))
		{
			text->writeDebugOutput("\nspriteHandler->End(): FAILED");
		}

		endDirectXFrameRendering();

	}

	// WE'VE LOST THE GPU, SLEEP UNTIL WE GET IT BACK
	else if (result == D3DERR_DEVICELOST) 
	{ 
		spriteHandler->OnLostDevice();
		textFont->OnLostDevice();
		Sleep(100); 
	}

	// WE'VE GOT IT BACK, RELOAD EVERYTHING. NOTE THAT
	// WE'LL ONLY GET THIS MESSAGE ONCE.
	else if (result == D3DERR_DEVICENOTRESET)
	{ 
		if (FAILED(graphicsDevice->Reset(&presentParameters)))
		{
			game->getText()->writeDebugOutput("\ngraphicsDevice->Reset: FAILED - Reloading GPU images");
			game->reloadAllDevices();
		}
		else
		{
			spriteHandler->OnResetDevice();
			textFont->OnResetDevice();
		}
	}
}


/*
	renderTextToDraw - This method renders a single piece of
	text to the screen using our EG font.
*/
void DirectXGraphics::renderTextToDraw(TextToDraw *textToDraw)
{
	textRect.left = textToDraw->x;
	textRect.right = textToDraw->x + textToDraw->width;
	textRect.top = textToDraw->y;
	textRect.bottom = textToDraw->y + textToDraw->height;
	LPCWSTR lpcwstrText = (*textToDraw->getText()).c_str();
	if (FAILED(textFont->DrawText (
				spriteHandler, 
				lpcwstrText, 
				-1, 
				&textRect, 
				DT_LEFT, 
				fontColor )))
		game->getText()->writeDebugOutput("\ntextFont->DrawText: FAILED");
}

/*
	setColorKey - This sets the color key to be used for loading images.
*/
void DirectXGraphics::setColorKey(int r, int g, int b)
{
	colorKey = D3DCOLOR_XRGB(r, g, b);
}

/*
	setFontColor - This sets the color to be used for rendering text.
*/
void DirectXGraphics::setFontColor(int r, int g, int b)
{
	fontColor = D3DCOLOR_XRGB(r, g, b);
}

/*
	shutdownGraphics - This method releases the DirectX objects we've created
	so that other applications can use the GPU. This should only be called
	when the application is closing.
*/
void DirectXGraphics::shutdown()
{
	if (textFont)
	{
		textFont->Release();
		textFont = NULL;
	}

	if (spriteHandler)
	{
		spriteHandler->Release();
		spriteHandler = NULL;
	}

	if (graphicsDevice)
	{
		graphicsDevice->Release();
		graphicsDevice = NULL;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = NULL;
	}}

/*
	startDirectXFrameRendering - This does some setup for rendering, like locking
	the GPU. Only one thread at a time can have a lock on the GPU.
*/
void DirectXGraphics::startDirectXFrameRendering()
{
	// CLEAR OUT ALL THE OLD RENDERING
    if (FAILED(graphicsDevice->Clear(0, NULL, D3DCLEAR_TARGET, BACKGROUND_COLOR, 1.0f, 0)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Clear(): FAILED");

	// ALLOWS DRAWING TO START, LOCKING THE GPU
    if (FAILED(graphicsDevice->BeginScene()))
		game->getText()->writeDebugOutput("\ngraphicsDevice->BeginScene(): FAILED");
}
