/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameText.h

	This class allows for easy rendering of text
	to the game screen.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class Game;
class TextGenerator;

const int MAX_TEXT_OBJECTS = 10;

class TextToDraw
{
private:
	wstring *text;

public:
	TextToDraw()	{}
	~TextToDraw()	{}
	int maxChars;
	int numChars;
	int x;
	int y;
	int width;
	int height;

	void setText(wstring *textToSet)
	{
		text = textToSet;
	}
	wstring* getText() { return text; }
};

class GameText
{
private:
	int textCounter;
	vector<TextToDraw*> *textToDraw;
	TextGenerator *textGenerator;

	// USED TO PRINT DEBUGGING OUTPUT
	TextFileWriter writer;

public:
	// INLINED ACCESSOR METHODS
	int				getNumTextObjectsToDraw()		{	return textToDraw->size();		}
	TextGenerator*	getTextGenerator()				{	return textGenerator;			}
	TextToDraw*		getTextToDrawAtIndex(int index)	{	return textToDraw->at(index);	}	

	// INLINED MUTATOR METHOD
	void setTextGenerator(TextGenerator *initTextGenerator)
	{
		textGenerator = initTextGenerator;
	}

	// METHODS DEFINED IN GameText.cpp
	GameText();
	~GameText();
	void addText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void changeTextOnly(wstring *textToAdd, int index);
	void initDebugFile(string debugFileName);
	void moveText(int index, int xMove, int yMove);
	void writeDebugOutput(string output);
};