/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WStringTable.cpp

	See WStringTable.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\WStringTable.h"

/*
	clear - This method clears out the string table vector, deleting
	all of the contents.
*/
void WStringTable::clear()
{
	wStringTable.clear();
}

/*
	getIndexOfWString - This method searches for the key
	parameter in the wstring table. If found, it returns it's index, if
	not found it returns the max unsigned int value;
*/
unsigned int WStringTable::getIndexOfWString(wstring key)
{
	unsigned int counter = 0;
	vector<wstring>::iterator it = wStringTable.begin();
	while(it != wStringTable.end())
	{
		if (key.compare((*it)) == 0)
			return counter;
		counter++;
		it++;
	}
	unsigned int max = 0;
	max -= 1;
	return max;
}

/*
	isInTable - tests to see if the testWString argument is already in
	the wstring table. If it is, true is returned, else false.
*/
bool WStringTable::isInTable(wstring testWString)
{
	unsigned int index = getIndexOfWString(testWString);
	if (index < wStringTable.size())
		return true;
	else
		return false;
}

/*
	putStringInWStringTable - This method is for adding elements
	to the wstring table. Since the wstring table only allows for
	single copies of each element, this method first checks to 
	see if the key wstring is already inside. If it is, it simply
	returns its index. If it's not found, it adds it and returns
	the index of the location where it was added.
*/
unsigned int WStringTable::putWStringInTable(wstring key)
{
	if (isInTable(key))
		return getIndexOfWString(key);
	wStringTable.push_back(key);
	return wStringTable.size()-1;
}
