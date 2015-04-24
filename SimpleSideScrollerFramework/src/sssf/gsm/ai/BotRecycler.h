/**
	This class provides premade lists of commonly used bots to avoid
	dynamic construction during gameplay. 

**/
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"

static const unsigned int RECYCLABLE_BOT_INCREMENT = 10;

class BotRecycler
{
private:
	map<wstring, list<Bot *> *> recyclableBots;
	map<wstring, Bot *> registeredBotTypes;

public:
	BotRecycler();
	~BotRecycler();
	void addMoreBots(list<Bot *> *botsNeededList, wstring botType, unsigned int numToAdd);
	void initRecyclableBots(wstring botType, unsigned int maxBotsNeeded);
	bool isRegisteredBotType(wstring botType);
	void recycleBot(wstring botType, Bot* botToRecycle);
	void registerBotType(wstring botType, Bot *sampleBot);
	Bot* retrieveBot(wstring botType);
};