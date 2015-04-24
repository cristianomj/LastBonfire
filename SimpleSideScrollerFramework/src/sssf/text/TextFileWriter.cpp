/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileWriter.cpp

	See TextFileWriter.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\text\TextFileWriter.h"

/*
	TextFileWriter - This constructor doesn't initialize a stream, so it just
	marks that it cannot currently write.
*/
TextFileWriter::TextFileWriter()
{
	outStreamOpen = false;
}

/*
	~TextFileWriter - This destructor makes sure the text
	stream is closed before destroying it and the file name
	pointers.
*/
TextFileWriter::~TextFileWriter()
{
	closeWriter();
}

/*
	closeWriter - This method closes the stream used for 
	reading from a file and then deletes the inFile stream. If
	you want to read again from the stream, you must call initFile
	first, which will re-initialize everything.
*/
void TextFileWriter::closeWriter()
{
	if (outStreamOpen)
		outFile.close();
}

/*
	initFile - This method initializes the stream for writing
	to a file using the file name provided as an argument. After
	calling this method, we are ready to write text.
*/
void TextFileWriter::initFile(string initFileName)
{
	fileName = initFileName;
	outFile.open(fileName);
	outStreamOpen = true;
}

/*
	writeText - This method appends the textToWrite argument
	to the end of the text file this object writes to.
*/
void TextFileWriter::writeText(string textToWrite)
{	
	outFile.write(textToWrite.c_str(), textToWrite.size());
}