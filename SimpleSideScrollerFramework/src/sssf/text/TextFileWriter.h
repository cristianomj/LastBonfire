/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileWriter.h

	This is a wrapper class for writing text to a file.
	Use the init method to initialize the stream. Text 
	can then be written to the file using the writeText
	method. Re-init the object to write to a new file.
	Also, remember to delete pointers of this object
	when done.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class TextFileWriter
{
private:
	// THE NAME OF THE TEXT FILE THIS OBJECT WILL WRITE TO
	string fileName;

	// THE OUTPUT STREAM WE WILL USE TO WRITE
	ofstream outFile;

	// THIS KEEPS TRACK OF WHEN THE STREAM IS ALL SETUP AND IT'S SAFE TO WRITE TEXT
	bool outStreamOpen;

public:
	// METHODS DEFINED IN TextFileWriter.cpp
	TextFileWriter();
	~TextFileWriter();
	void closeWriter();
	void initFile(string initFileName);
	void writeText(string textToWrite);
};