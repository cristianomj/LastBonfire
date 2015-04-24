#include "xmlfi_VS\stdafx.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "xmlfi\XMLFileImporter.h"

const char* XMLFileImporter::newCharArrayFromString(string text)
{
	char *charArray = new char[text.size() + 1];
	int unsigned i = 0;
	for (; i < text.size(); i++)
		charArray[i] = text[i];
	charArray[i] = '\0';
	return charArray;
}

const char* XMLFileImporter::newCharArrayFromWstring(wstring wideText)
{
	char *charArray = new char[wideText.size() + 1];
	unsigned int i = 0;
	for ( ; i < wideText.size(); i++)
		charArray[i] = (char)wideText[i];
	charArray[i] = '\0';
	return charArray;
}

int XMLFileImporter::extractIntAtt(const TiXmlElement *pElem, string att)
{
	stringstream ss;
	int num;
	const char* text;
	text = pElem->Attribute(att.c_str());
	ss << text;
	ss >> num;
	ss.str("");
	return num;
}

const char* XMLFileImporter::extractCharAtt(const TiXmlElement *pElem, string att)
{
	stringstream ss;
	const char* text;
	text = pElem->Attribute(att.c_str());
	return text;
}

bool XMLFileImporter::extractBoolAtt(const TiXmlElement *pElem, string att)
{
	const char* text;
	text = pElem->Attribute(att.c_str());
	const char trueText[] = "true";
	if (strcmp(text, trueText) == 0)
		return true;
	else
		return false;
}