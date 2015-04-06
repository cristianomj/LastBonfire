#pragma once
#include "last_bonfire_VS\stdafx.h"
#include "sssf\text\TextGenerator.h"

class LastBonfireTextGenerator : public TextGenerator
{
private:
	wstring textToGenerate;

public:
	LastBonfireTextGenerator() {}
	~LastBonfireTextGenerator() {}

	// DEFINED IN LastBonfireTextGenerator.cpp
	void appendMouseCoords(Game *game);
	void initText(Game *game);
	void updateText(Game *game);
};