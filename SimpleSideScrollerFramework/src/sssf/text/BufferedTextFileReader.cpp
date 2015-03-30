/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BufferedTextFileReader.cpp

	See BufferedTextFileReader.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\text\BufferedTextFileReader.h"

/*
	Default Constructor - We don't know the name of the file
	to use yet, so we'll initialize everything to NULL.
*/
BufferedTextFileReader::BufferedTextFileReader()
{
	moreTextToRead = false;
}

/*
	Deconstructor - Make sure the file stuff doesn't cause
	a memory leak.
*/
BufferedTextFileReader::~BufferedTextFileReader()
{
	closeReader();
}

/*
	closeReader - This method closes the stream used for 
	reading from a file and then deletes the inFile stream. If
	you want to read again from the stream, you must call initFile
	first, which will re-initialize everything.
*/
void BufferedTextFileReader::closeReader()
{
	inFile.close();
	moreTextToRead = false;
}

/*
	initFile - This method initializes the stream for reading
	from a file using the file name provided as an argument. After
	calling this method, we are ready to read text.
*/
void BufferedTextFileReader::initFile(wstring initFileName)
{
	fileName = initFileName;
	inFile.clear();
	inFile.open(fileName);
	loadBuffer();
	moreTextToRead = true;
}

/*
	loadBuffer - This method reads all the text in the fileName file and
	loads it into the buffer, one line at a time.
*/
void BufferedTextFileReader::loadBuffer()
{
	streamsize size = inFile.rdbuf()->in_avail();
	wstring inputLine;
	while(!inFile.eof())
	{
		getline(inFile, inputLine);
		buffer.push_back(inputLine);
	}
	lineCounter = 0;
}

/*
	readLineOfText - This method reads a line of text from the
	file and returns it. Note that before calling this method, first
	call initFile to setup the stream. Note that this method is
	allocating memory on the heap for 256 characters for each line,
	so that should not be exceeded. Also, remember to delete the
	line when done using it to avoid memory leaks.
*/
wstring BufferedTextFileReader::getNextLine()
{
    if (moreTextToRead)
	{
		wstring lineOfText = buffer[lineCounter];
		lineCounter++;
		if (lineCounter == buffer.size())
			moreTextToRead = false;
		return lineOfText;
	}
	else
        return NULL;
}