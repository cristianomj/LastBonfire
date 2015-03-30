#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\text\BufferedTextFileReader.h"

/*
	loadGameProperties - Called only once, when the application starts, this method
	loads all the delimited properties into the map argument, which the calling
	method may then take and use to setup the game application.
*/
void GameDataLoader::loadGameProperties(	Game *game, 
											map<wstring, wstring> *properties,
											wstring gameInitFile)
{
	// OPEN THE READER AND LOAD IT WITH
	// THE CONTENTS OF gameInitFile
	BufferedTextFileReader reader;
	reader.initFile(gameInitFile);
	
	// NOW WE CAN READ IN ALL THE GAME DATA. NOTE THAT THE 
	// GAME DATA FILE WE ARE READING NEEDS TO BE IN THE
	// PROPER FORMAT, OBVIOUSLY
	wstring line;

	// LOAD ALL THE PROPERTIES
	while (reader.hasMoreLines())
	{
		line = reader.getNextLine();
		loadProperty(&line, properties);
	}
}

/*
	loadProperty - this method loads the line argument as a delimited
	name,value pair in to the 
*/
void GameDataLoader::loadProperty(	wstring *line,
									map<wstring,wstring> *properties)
{
	unsigned int delimiterIndex = line->find(DEFAULT_DELIMITER);
	wstring name = line->substr(0, delimiterIndex);
	wstring value = line->substr(delimiterIndex+1);
	(*properties)[name] = value;
}