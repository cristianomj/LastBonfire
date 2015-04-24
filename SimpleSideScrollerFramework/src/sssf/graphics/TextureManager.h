/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextureManager.h

	This class provides the foundation for technology specific classes
	that will manage game images. Different child classes may use different
	graphics libraries (i.e. DirectX, OpenGL) to load images into the GPU.
	This class provides a framework that all such classes should follow.

	TextureManagers maintain a data structure of images mapped to file names.
	It also has a WStringTable, which maps file names to indices. The point of
	this is to allow mutliple game elements to share images where the images
	only need to be loaded into memory once, in the texture manager. It is up
	to the child classes to define the image-wchar_t mappings because the image
	types differ based on the technology used.

	In rendering things like game world backgrounds, we may wish to tile
	textures, repeating certain images over and over. This allows for us to
	efficiently render a large world without having to load giant images.

	Since TextureManagers map the textures to file names (and path), they
	should also be able to reload all the images they store if necessary.
	This may be necessary when the GPU device is lost and then regained.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\text\TextFileWriter.h"

class GameGraphics;

class TextureManager
{
protected:
	// WE MAY NEED THE GRAPHICS OBJECT TO HELP WITH LOADING IMAGES
	GameGraphics *graphics;

	// THE wstring TABLE SHOULD MAP FILE PATH/NAME TO INDEX
	WStringTable wstringTable;

public:
	// INLINED ACCESSOR METHOD
	WStringTable*	getWStringTable()	{ return &wstringTable;	}

	// INLINED MUTATOR METHOD
	void setGraphics(GameGraphics *initGraphics)
	{
		graphics = initGraphics;
	}

	// VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
	// SPECIFIC CLASSES
	virtual void			clear()=0;
	virtual unsigned int	loadTexture(wstring key)=0;
	virtual void			reloadAllTextures()=0;
	virtual bool loadTileSetFromTexture(Game *game,	wstring dir, wstring sourceImage, int tileWidth, int tileHeight)=0;

	// CONCRETE METHODS DEFINED IN TextureManager.cpp
	TextureManager();
	~TextureManager();
};