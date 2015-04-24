/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Button.h

	This represents a single button in a game GUI. In addition
	to storing information about it's presentation, like the
	x, y location, width & height, and image id from the
	gui texture manager, it also stores a pointer to the
	event handler that will be used when this button is 
	clicked. This is the button's ButtonEventHandler.
	
	Buttons can have either 1 or 2 images. The two images
	being a normal image and a mouse over image. If only
	one image id is provided, it is used for both states.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class ButtonEventHandler;
class Game;
class GameInput;
class TextureManager;

class Button
{
private:
	// INDEX IN THE GUI TEXTURE MANAGER OF THE NORMAL BUTTON IMAGE
	int normalTextureID;

	// INDEX IN THE GUI TEXTURE MANAGER OF THE MOUSE-OVER BUTTON IMAGE
	int mouseOverTextureID;

	// OTHER RENDERING VARIABLES
	int x;
	int y;
	int z;
	int alpha;
	int width;
	int height;
	bool mouseOver;

	// EACH BUTTON HAS A command, AND WHEN WE DETECT A MOUSE CLICK ON THE
	// BUTTON, WE SEND THE command TO THE eventHandler, WHICH PROVIDES
	// A PROGRAMMED RESPONSE
	wstring command;
	ButtonEventHandler *eventHandler;

public:
	// INLINED ACCESSOR METHODS
	int		getNormalTextureID()	{ return normalTextureID;		}
	int		getMouseOverTextureID()	{ return mouseOverTextureID;	}
	int		getX()					{ return x;						}
	int		getY()					{ return y;						}
	int		getZ()					{ return z;						}
	int		getAlpha()				{ return alpha;					}
	int		getHeight()				{ return height;				}
	int		getWidth()				{ return width;					}
	bool	isMouseOver()			{ return mouseOver;				}
	wstring	getCommand()			{ return command;				}

	// INLINED MUTATOR METHODS
	void setNormalTextureID(int initNormalTextureID)
	{ normalTextureID = initNormalTextureID;		}
	void setMouseOverTextureID(int initMouseOverTextureID)
	{ mouseOverTextureID = initMouseOverTextureID;	}
	void setX(int initX)
	{ x = initX;									}
	void setY(int initY)
	{ y = initY;									}
	void setZ(int initZ)
	{ z = initZ;									}
	void setAlpha(int initAlpha)
	{ alpha = initAlpha;							}
	void setWidth(int initWidth)
	{ width = initWidth;							}
	void setHeight(int initHeight)
	{ height = initHeight;							}
	void setCommand(wstring changeCommand)
	{ command = changeCommand;						}
	void setEventHandler(ButtonEventHandler *initEventHandler)
	{ eventHandler = initEventHandler;				}


	// METHODS DEFINED IN Button.cpp
	Button::Button();
	Button::~Button();
	void fireEvent(Game *game);
	void initButton(int initNormalTextureID, int initMouseOverTextureID, int initX, int initY, int initZ, int initAlpha, int initWidth, int initHeight, bool initMouseOver, wstring initCommand);
	void updateMouseOver(long mouseX, long mouseY);
};
