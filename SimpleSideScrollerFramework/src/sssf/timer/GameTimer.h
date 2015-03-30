/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameTimer.h

	This class provides all game timing. You give it 
	a target frame rate, and then once per frame cal
	timeGameLoop. This will clamp the frame rate to
	your target loop. If the loop is running slower
	than the target rate, you'll have to either settle
	for a slower game, which means you should improve
	performance in other parts of the game, or you should
	scale all game movements proportionally to the 
	slowdown.

	This timer works by calculating the total time a given
	frame has taken and comparing it to the target framerate.
	It will then sleep for the difference if the measured
	framerate is faster than the target.

	Note that this class is just a framework. Subclasses
	should provide platform-specific impementations.
*/

#pragma once
#include "sssf_VS\stdafx.h"


class GameTimer
{
public:
	// INLINED METHODS
	GameTimer() {}
	~GameTimer(){}

	// METHODS DEFINED IN CHILD CLASSES, WHICH WOULD BE PLATFORM SPECIFIC
	virtual int getLoopCounter()=0;
	virtual unsigned int getSleeplessTotalTime()=0;
	virtual float getTimeScaleFactor()=0;
	virtual unsigned int getTotalTime()=0;
	virtual unsigned int calculateAverageSleeplessFramerate()=0;
	virtual unsigned int calculateAverageActualFramerate()=0;
	virtual unsigned int calculateAverageSleepTime()=0;
	virtual unsigned int getMinSystemTimerResolution()=0;
	virtual void resetTimer()=0;
	virtual void timeGameLoop()=0;
};