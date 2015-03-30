/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WStringTable.h

	This class manages an array of wstrings in a vector. Basically it is
	useful for storing the names of loaded resources so that all the game
	objects don't have to. They can instead just store the indices of the
	resources in this table in order to save memory and trade in the id
	for the resource when needed.

	Note that this should only be used under all or nothing circumstances.
	What that means is we load all the strings at once and then will not
	add any more for the duration of its use unless we clear and then 
	load a whole new batch. The reason is that we will keep the wstrings
	sorted to optimize search and once loaded, we don't want to change 
	the indices of any of our assets in the table.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class WStringTable
{
private:
	vector<wstring> wStringTable;

public:
	// INLINED ACCESSOR METHOD
	wstring getWStringFromTable(unsigned int index)	{	return wStringTable[index];	}

	// NO DATA TO INITIALIZE OR DESTROY
	WStringTable()	{}
	~WStringTable()	{}

	// METHODS DEFINED IN WStringTable.cpp
	void			clear();
	bool			isInTable(wstring testWString);
	unsigned int	getIndexOfWString(wstring key);
	unsigned int	putWStringInTable(wstring key);
	unsigned int	getNumWStringsInTable() { return wStringTable.size(); }
};