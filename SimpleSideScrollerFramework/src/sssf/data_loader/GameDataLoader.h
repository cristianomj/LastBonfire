/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameDataLoader.h

	This class should read data from CSV (Comma Separated Values)
	files and uses them to load game data for a game
	application. Game data for rendering the game GUI is
	loaded when the game starts, data for displaying each
	game level would be loaded as needed. Note that we are not
	using this in the first benchmark.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"

const static wchar_t DEFAULT_DELIMITER = ':';

class GameDataLoader
{
private:
	// THIS FILE CONTAINS THE LOCATION OF ALL
	// GAME DATA TO BE LOADED
	wstring				gameDataPathName;

public:
	// SIMPLE INLINED CONSTRUCTOR/DESTRUCTOR
	GameDataLoader()			{} 
	~GameDataLoader()			{}

	// ACCESSOR AND MUTATOR
	wstring getGameDataPathName()	{ return gameDataPathName; }
	void setGameDataPathName(wstring initGameDataPathName)
	{
		gameDataPathName = initGameDataPathName;
	}

	// METHOD DEFINED IN GameDataLoader.cpp
	void loadGameProperties(	Game *game, 
								map<wstring, wstring> *properties,
								wstring gameInitFile);
	void loadProperty(	wstring *text,
						map<wstring,wstring> *properties);

	// METHODS TO BE DEFINED INSIDE CHILD CLASSES
	virtual void loadGame(Game *game, wstring gameInitFile)=0;
	virtual void loadGUI(Game *game, wstring guiInitFile)=0;
	virtual void loadWorld(Game *game, wstring levelInitFile)=0;
};