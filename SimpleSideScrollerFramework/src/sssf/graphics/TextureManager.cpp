/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextureManager.cpp

	See TextureManager.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\TextureManager.h"

/*
	TextureManager - Default Constructor, it creates the WStringTable.
*/
TextureManager::TextureManager()  
{

}

/*
	~TextureManager - Destructor, it cleans up the WStringTable pointer.
*/
TextureManager::~TextureManager() 
{
	// TODO Delete strings in string table
}