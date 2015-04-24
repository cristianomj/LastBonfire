/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsTimer.cpp

	See WindowsTimer.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\platforms\Windows\WindowsTimer.h"
#include "sssf\text\TextFileWriter.h"
#include "sssf\timer\GameTimer.h"

/*
	WindowsTimer - Default constructor, this method
	initializes the timer resolution to the best the
	system has to offer. In order to start timing,
	call the resetTimer method outside the main game
	loop, right before it starts. The timeGameLoop
	method is the only one needed inside the game
	loop.
*/
WindowsTimer::WindowsTimer()
{
	// LET'S START WITH A TARGET FPS OF 33 FRAMES PER SECOND,
	// THAT'S 30 ms PER FRAME
	targetFPS = 33;

	// GET INFORMATION ABOUT HOW GOOD 
	// THE SYSTEM'S TIMER IS
	timerResolution = getMinSystemTimerResolution();

	// AND THEN USE THAT RESOLUTION FOR ALL SUBSEQUENT TIMING
	timeBeginPeriod(timerResolution);

	// CALCULATE THE NUMBER OF MILLISECONDS WE
	// WANT PER FRAME ACCORDING TO THE TARGET_FPS
	targetMillisPerFrame = (DWORD)(1000/targetFPS);
}

/*
	~WindowsTimer - Destructor, we only have to clean up
	the writer, since we declared it as a pointer instance
	variable.
*/
WindowsTimer::~WindowsTimer() {}

/*
	calculateAverageActualFramerate - This method
	calculates how long on average a frame takes in
	the game loop, including time spent sleeping.
*/
unsigned int WindowsTimer::calculateAverageActualFramerate()
{
	if (loopCounter == 0)
		return DWORD(0);
	else
	{
		int averageTime = totalTime/loopCounter;
		if (averageTime != 0)
			return DWORD(1000/averageTime);
		else
			// IT IS AT LEAST 1000, BUT WE REALLY
			// DON'T KNOW DUE TO TIMER RESOLUTION
			return DWORD(1000);
	}
}

/*
	calculateAverageSleeplessFramerate - This method
	calculates how long on average a frame takes in
	the game loop, not including time spent sleeping.
*/
unsigned int WindowsTimer::calculateAverageSleeplessFramerate()
{
	if (loopCounter == 0)
		return DWORD(0);
	else
	{
		float averageTime = (float)sleeplessTotalTime/(float)loopCounter;
		if ((int)averageTime != 0)
			return DWORD(1000/averageTime);
		else
			// IT IS AT LEAST 1000, BUT WE REALLY
			// DON'T KNOW DUE TO TIMER RESOLUTION
			return DWORD(1000);
	}
}

/*
	calculateAverageSleepTime - This method calculates
	on average how much time is spent sleeping each
	frame since the game loop started.
*/
unsigned int WindowsTimer::calculateAverageSleepTime()
{
	if (loopCounter == 0)
		return DWORD(0);
	else
		return totalSleepTime/loopCounter;
}

/*
	getMinSystemTimeResolution - This method queries
	the local operating system and asks how good the
	system resolution is for timing purposes. The best
	we can expect to be returned would be 1 millisecond.
*/
unsigned int WindowsTimer::getMinSystemTimerResolution()
{
	TIMECAPS timer;
	timeGetDevCaps(&timer, sizeof(TIMECAPS));
	UINT min = timer.wPeriodMin;
	return min;
}

/*
	resetTimer - This method resents all variables used
	for compiling statistics as well as getting the first
	start times for the first iteration through the
	game loop. This should be placed outside the game
	loop, right before it is to start.
*/
void WindowsTimer::resetTimer()
{
	gameLoopStartTime = timeGetTime();
	sleeplessGameLoopStartTime = timeGetTime();
	loopCounter = 0;
	totalTime = 0;
	sleeplessTotalTime = 0;
	totalSleepTime = 0;
	actualLoopRate = 0;
	sleeplessLoopRate = 0;
}

/*
	timeGameLoop - This method gets the current time
	and then calculates the difference between now
	and the last time it got the time. This is the
	game time, which can then be used to calculate
	the current frame rate. If the current frame rate
	is too fast, we sleep for a little to clamp it 
	to the TARGET_FPS. If it is too slow, we calculate
	a scaling factor that can be used for moving
	sprites and for scrolling.
*/
void WindowsTimer::timeGameLoop()
{
	// GET THE END OF FRAME TIME
	gameLoopEndTime = timeGetTime();

	// HOW MUCH TIME PASSED DURING THE LAST FRAME?
	loopTime = gameLoopEndTime - gameLoopStartTime;

	// GET THE START TIME FOR NEXT FRAME, IF THERE IS ONE
	gameLoopStartTime = timeGetTime();

	// ADD THE LAST FRAME'S TIME TO THE TOTAL
	totalTime += loopTime;
	
	// HOW MUCH TIME PASSED NOT INCLUDING
	// OUR FORCED SLEEPING?
	sleeplessLoopTime = gameLoopEndTime - sleeplessGameLoopStartTime;

	// ADD THE LAST FRAME'S SLEEPLESS TIME TO THE TOTAL
	sleeplessTotalTime += sleeplessLoopTime;

	if (loopTime == DWORD(0))
		actualLoopRate = DWORD(1000);
	else						
		actualLoopRate = 1000 / loopTime;

	if (sleeplessLoopTime == DWORD(0))
		actualLoopRate = DWORD(1000);
	else
		sleeplessLoopRate = 1000 / sleeplessLoopTime;

	// IF THIS PAST FRAME RAN TOO FAST IT'S
	// LIKELY THE NEXT FRAME WILL RUN FAST ALSO
	if (targetMillisPerFrame > sleeplessLoopTime)
	{
		// SO LET'S CLAMP IT TO OUR TARGET FRAME RATE
		sleepTime = targetMillisPerFrame - sleeplessLoopTime;
		totalSleepTime += sleepTime;
		Sleep(sleepTime);
		timeScaleFactor = 1;
	}
	else
	{
		sleepTime = 0;

		// WE MIGHT USE THIS timeScaleFactor TO SCALE
		// MOVEMENTS OF GAME SPRITES AND SCROLLING TO
		// MAKE UP FOR THE SLOWING DOWN OF THE GAME LOOP
		timeScaleFactor = ((float)targetFPS)/
						((float)sleeplessLoopRate);	
	}

	// GET THE START TIME FOR THE LOOP
	// NOT INCLUDING THE SLEEP TIME FROM THE LAST LOOP
	sleeplessGameLoopStartTime = timeGetTime();
	loopCounter++;
}