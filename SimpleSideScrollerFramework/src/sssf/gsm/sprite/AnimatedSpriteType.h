/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSpriteType.h

	This class stores data to be shared between similar types
	of sprites. Individual sprites will be in different locations
	and have different state variables, but they may share things
	like animation sequences.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class AnimatedSpriteType
{
private:
	// EACH SPRITE TYPE HAS A UNIQUE ID NUMBER
	int spriteTypeID;

	// animationSequences WILL BE A map OF JAGGED vectors STORING THE IMAGE FLIPPING
	// DATA FOR EACH ANIMATION STATE. BASICALLY WHAT IT IS IS EACH (key,value) PAIR IN THE MAP
	// RESPRESENTS A NAMED ANIMATION STATE, LIKE "IDLE", WHICH WOULD BE THE KEY. THE
	// VALUE WOULD BE THE VECTOR STORING A SERIES OF (IMAGE ID, FRAMES DURATION) TUPLES 
	// IN THE ANIMATION SEQUENCE FOR THAT GIVEN STATE. EACH TUPLE MEANS DISPLAY THE
	// IMAGE ID FOR FRAMES DURATION NUMBER OF FRAMES
	map<wstring, vector<unsigned int>*> animationSequences;

	// WIDTH & HEIGHT OF TEXTURES
	int textureHeight, textureWidth;

public:
	// INLINED ACCESSOR METHODS
	unsigned int getSequenceSize(wstring state)	{ return animationSequences[state]->size();		}
	unsigned int getSpriteTypeID()				{ return spriteTypeID;							}
	int	getTextureHeight()						{ return textureHeight;							}
	int	getTextureWidth()						{ return textureWidth;							}

	// INLINED MUTATOR METHODS
	void setSpriteTypeID(int initSpriteTypeID)
	{	spriteTypeID = initSpriteTypeID;							}
	void setTextureSize(int initTextureWidth,	int initTextureHeight)
	{	textureWidth = initTextureWidth;
		textureHeight = initTextureHeight;							}

	// METHODS DEFINED IN AnimatedSpriteType.cpp
	AnimatedSpriteType();
	~AnimatedSpriteType();
	void			addAnimationFrame(wstring animationState, unsigned int imageId, unsigned int duration);
	void			addAnimationSequence(wstring animationState);
	unsigned int	getAnimationFrameID(wstring animationState, int frameIndex);
	unsigned int	getDuration(wstring animationState, unsigned int frameIndex);
};