/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameOS.h

	This class provides a framework for managing OS messages between
	this application and the Operating System. Each platform would have
	its own implementation.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class GameOS
{
public:
	virtual void processOSMessages()=0;
	virtual void shutdown()=0;
};
