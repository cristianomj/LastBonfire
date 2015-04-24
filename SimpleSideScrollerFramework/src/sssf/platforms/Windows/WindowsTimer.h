/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsTimer.h

	This class provides a Windows platform implementation
	for all game timing. You give it 
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
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\text\TextFileWriter.h"
#include "sssf\timer\GameTimer.h"

class WindowsTimer : public GameTimer
{
private:
	// TARGET FRAMES PER SECOND
	int targetFPS;

	// THE MINIMUM TIMER INCREMENT
	UINT timerResolution;

	// THE INVERSE OF THE TARGET FRAME RATE IN MILLISECONDS
	DWORD targetMillisPerFrame;

	// THE TIME TAKEN AT THE START OF THE FRAME,
	// INCLUDING AND NOT INCLUDING SLEEP TIME
	DWORD gameLoopStartTime;
	DWORD sleeplessGameLoopStartTime;

	// THE TIME TAKEN AT THE END OF THE FRAME
	DWORD gameLoopEndTime;

	// THE TOTAL TIME A SINGLE FRAME TOOK TO ITERATE,
	// INCLUDING AND NOT INCLUDING SLEEP TIME
	DWORD loopTime;
	DWORD sleeplessLoopTime;

	// THE TOTAL TIME TAKEN SINCE ENTERING THE GAME LOOP,
	// INCLUDING AND NOT INCLUDING SLEEP TIME
	DWORD totalTime;
	DWORD sleeplessTotalTime;

	// THE FRAME RATE OF THE LATEST FRAME,
	// INCLUDING AND NOT INCLUDING SLEEP TIME
	DWORD actualLoopRate;
	DWORD sleeplessLoopRate;

	// THE AMOUNT WE WILL SET TO SLEEP EACH FRAME
	// IF THE GAME IS RUNNING FASTER THAN THE TARGET FPS
	DWORD sleepTime;

	// THE TOTAL AMOUNT OF TIME WE HAVE MADE OUR APPLICATION
	// SLEEP SINCE THE TIMER WAS RESET
	DWORD totalSleepTime;

	// WHEN A LOOP RUNS TOO FAST, WE CLAMP OUR FRAME RATE,
	// SO THIS FACTOR IS SIMPLY 1, BUT WHEN OUR LOOP RUNS
	// TO SLOW, THIS INCREASES PROPORTIONATELY SUCH THAT
	// WE CAN SCALE ALL MOVEMENTS OF SPRITES & SCROLLING
	float timeScaleFactor;

	// NUMBER OF ITERATIONS, USED FOR CALCULATIONS
	int loopCounter;

	// FOR WRITING OUR TIMER STATS TO A TEXT FILE
	TextFileWriter *writer;

public:
	// INLINED METHODS
	int	  getLoopCounter()					{ return loopCounter;		}
	unsigned int getSleeplessTotalTime()	{ return sleeplessTotalTime;}
	float getTimeScaleFactor()				{ return timeScaleFactor;	}
	unsigned int getTotalTime()				{ return totalTime;			}
	int getTargetFPS()						{ return targetFPS;			}
	void setTargetFPS(int initTargetFPS) 
	{	targetFPS = initTargetFPS; 
		targetMillisPerFrame = (DWORD)(1000/targetFPS);
	}

	// METHODS DEFINED IN WindowsTimer.cpp
	WindowsTimer();
	~WindowsTimer();
	unsigned int calculateAverageSleeplessFramerate();
	unsigned int calculateAverageActualFramerate();
	unsigned int calculateAverageSleepTime();
	unsigned int getMinSystemTimerResolution();
	void	resetTimer();
	void	timeGameLoop();
};