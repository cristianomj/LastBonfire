/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	IllegalFileFormatException.h

	This class represents an exception thrown when data
	from a file that is being loaded is in the wrong format.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class IllegalFileFormatException:exception
{
public:
	// INLINED CONSTRUCTOR
	IllegalFileFormatException(string *errorMessage)
	{
		exception(errorMessage->c_str());
	}

	// INLINED DESTRUCTOR
	~IllegalFileFormatException(){}
};