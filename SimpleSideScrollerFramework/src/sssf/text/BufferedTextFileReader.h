/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileReader.h

	This is a wrapper class for reading text from a file. This
	can be useful for reading game design files for building a
	game world.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class BufferedTextFileReader
{
protected:
	// IS SOMETHING READY TO READ?
	bool moreTextToRead;

	// FILE NAME TO READ FROM
	wstring fileName;

	// STREAM TO WRITE TO
	wifstream inFile;

	// TEMP PLACE TO PUT DATA
	vector<wstring> buffer;

	// WE'LL USE THIS, BUT WE DON"T WANT ANYONE ELSE USING IT
	void	loadBuffer();

	// THIS WILL KEEP TRACK OF WHICH LINES IN THE BUFFER
	// HAVE BEEN ACCESSED
	unsigned int lineCounter;

public:
	// INLINED ACCESSOR
	bool	hasMoreLines()	{ return moreTextToRead; }

	// METHODS DEFINED INSIDE BufferedTextFileReader
	BufferedTextFileReader();
	~BufferedTextFileReader();
	void	closeReader();
	wstring getNextLine();	
	void	initFile(wstring initFileName);
};