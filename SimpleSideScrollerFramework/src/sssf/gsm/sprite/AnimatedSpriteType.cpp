/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSpriteType.cpp

	See AnimatedSpriteType.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"

/*
	AnimatedSpriteType - Nothing to construct. Note that the vectors storing
	the animation sequences are added dynamically, since each sprite type
	is completely different.
*/
AnimatedSpriteType::AnimatedSpriteType()  
{

}

/*
	~AnimationSpriteType - Destructor, it cleans up our pointers.
*/
AnimatedSpriteType::~AnimatedSpriteType() 
{
	// TODO  DELETE ALL THE VECTORS

}

/*
	addAnimationFrame - This method is for adding an animation (image,duration) tuple to
	an existing animation state for this type of sprite.
*/
void AnimatedSpriteType::addAnimationFrame(	wstring animationState, 
											unsigned int imageId, 
											unsigned int duration)
{	
	animationSequences.at(animationState)->push_back(imageId);
	animationSequences.at(animationState)->push_back(duration);
}

/*
	addAnimationSequence - This method is for adding a new animation state. It will setup
	a vector for storing the data, but will not initialize it with any.
*/
void AnimatedSpriteType::addAnimationSequence(wstring animationState)
{
	vector<unsigned int> *sequenceData = new vector<unsigned int>();
	animationSequences[animationState] = sequenceData;
}

/*
	getAnimationFrameID - For accessing the image id of the animation frame represented
	by the frameIndex value.
*/
unsigned int AnimatedSpriteType::getAnimationFrameID(wstring animationState, int frameIndex)
{
	return animationSequences.at(animationState)->at(frameIndex);
}

/*
	getDuration - For accessing the duration of the animation frame represented
	by the frameIndex value.
*/
unsigned int AnimatedSpriteType::getDuration(wstring animationState, unsigned int frameIndex)
{
	return animationSequences.at(animationState)->at(frameIndex+1);
}
