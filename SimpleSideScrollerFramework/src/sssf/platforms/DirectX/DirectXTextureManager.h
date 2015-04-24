/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DirectXTextureManager.h

	This class is a TextureManager class that does all 
	texture management using DirectX.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\TextureManager.h"

class DirectXTextureManager: public TextureManager
{
private:
	// HERE ARE OUR TEXTURES. NOTE THAT THE FILE NAME/PATH FOR ALL
	// TEXTURES ARE STORED IN THE INHERITED STRING TABLE AND THAT
	// THE INDICES OF THE FILE NAMES THERE MUST CORRESPOND TO THE
	// INDICES OF THESE TEXTURES
	map<wstring, LPDIRECT3DTEXTURE9> textures;

public:
	// INLINED ACCESSOR METHODS
	LPDIRECT3DTEXTURE9 getTexture(wstring fileName)
	{
		return textures.find(fileName)->second;
	}

	// METHODS DEFINED IN DirectXTextureManager.cpp
	DirectXTextureManager();
	~DirectXTextureManager();
	void				clear();
	unsigned int		loadTexture(wstring key);
	void				reloadAllTextures();

	// PLATFORM-SPECIFIC
	LPDIRECT3DTEXTURE9	getTexture(unsigned int id);
	unsigned int		fillTexture(wstring key, LPDIRECT3DTEXTURE9 *textureToFill);
	bool loadTileSetFromTexture(Game *game,	wstring dir, wstring sourceImage, int tileWidth, int tileHeight);
};