#pragma once

#include "xmlfi_VS\stdafx.h"

struct XMLDataDocNode
{
	map<string,string> attributes;
	map<string,vector<XMLDataDocNode>> childNodes;
};

struct XMLDataDoc
{

};

class XMLFileImporter
{
public:
	int extractIntAtt(const TiXmlElement *pElem, string att);
	const char* extractCharAtt(const TiXmlElement *pElem, string att);
	bool extractBoolAtt(const TiXmlElement *pElem, string att);
	const char* newCharArrayFromWstring(wstring wideText);
	const char* newCharArrayFromString(string text);
};