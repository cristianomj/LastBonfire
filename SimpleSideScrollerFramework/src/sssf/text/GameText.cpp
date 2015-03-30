/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameText.cpp

	See GameText.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\text\GameText.h"

/*
	GameText - Default constructor, it initializes the data structure for 
	storing all text and sets the default text color to white.
*/
GameText::GameText()
{
	textToDraw = new vector<TextToDraw*>();
}

/*
	~GameText - Destructor, it cleans up the textToDraw vector and the
	textGenerator.
*/
GameText::~GameText()
{
	delete textToDraw;
}

/*
	addText - This method adds text for rendering. Text only needs to
	be added once.
*/
void GameText::addText(wstring *textToAdd,
					   int initX,
					   int initY,
					   int initWidth,
					   int initHeight)
{
	TextToDraw *text = new TextToDraw();
	text->setText(textToAdd);
	text->x = initX;
	text->y = initY;
	text->width = initWidth;
	text->height = initHeight;
	textToDraw->push_back(text);
}

/*
	changeTextOnly - Once text is added, if we want to subsequently
	change it, we may use this method.
*/
void GameText::changeTextOnly(wstring *textToSet, int index)
{
	TextToDraw *textToChange = textToDraw->at(index);
	wstring *oldText = textToChange->getText();
	delete oldText;
	textToChange->setText(textToSet);
}

/*
	initDebugFile - This method sets up the writer to allow us to write
	to the debug output text file.
*/
void GameText::initDebugFile(string debugFileName)
{
	writer.initFile(debugFileName);
}

/*
	moveText - This method allows us to move the location
	of where we draw our text.
*/
void GameText::moveText(int index, int xMove, int yMove)
{
	int x = textToDraw->at(index)->x;
	int y = textToDraw->at(index)->y;
	textToDraw->at(index)->x = x + xMove;
	textToDraw->at(index)->y = y + yMove;
}

/*
	writeOutput - This method writes text to the debugging log file.
*/
void GameText::writeDebugOutput(string output)	
{	
	writer.writeText(output);
}