#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\ai\BotRecycler.h"

BotRecycler::BotRecycler()
{
	srand((unsigned int)time(NULL));
}

/*
	A BotRecycler is constructed only once while a game app is
	running, at startup. When it is deconstructed, it must delete
	all remaining bots.
*/
BotRecycler::~BotRecycler()
{
	// GO THROUGH ALL THE TYPES OF BOTS AND DELETE ALL OF THEM
}


void BotRecycler::initRecyclableBots(wstring botType, unsigned int maxBotsNeeded)
{
	// FIRST MAKE SURE WE DON'T ALREADY HAVE A BUNCH OF RECYCLABLE
	// BOTS FOR THIS TYPE. TO DO THIS, WE USE SOME C++ WEIRDNESS.
	map<wstring, list<Bot *> *>::iterator testIt = recyclableBots.find(botType);
	if (testIt == recyclableBots.end())
	{
		// MAKE THE RECYCLE LIST
		recyclableBots[botType] = new list<Bot *>();

		// AND ADD THE BOTS
		addMoreBots(recyclableBots[botType], botType, maxBotsNeeded);
	}
}

/*
	It is assumed that a game will have many different types of Bots, thinking
	in different ways, some of them even custom bots defined by the game app rather
	than by this framework. So, in order to make these bots recyclable, we have to
	register them with this recycler so that this recycler knows how to make new
	ones as needed.
*/
void BotRecycler::registerBotType(wstring botType, Bot *sampleBot)
{
	// FIRST MAKE SURE WE DON'T ALREADY HAVE A BUNCH OF RECYCLABLE
	// BOTS FOR THIS TYPE. TO DO THIS, WE USE SOME C++ WEIRDNESS.
	if (!isRegisteredBotType(botType))
	{
		// REGISTER THE BOT
		registeredBotTypes[botType] = sampleBot;
	}
}

bool BotRecycler::isRegisteredBotType(wstring botType)
{
	// FIRST MAKE SURE THIS IS A REGISTERED BOT TYPE,
	// IF IT IS NOT, WE NEED TO RETURN NULL
	map<wstring, Bot *>::iterator testIt = registeredBotTypes.find(botType);
	if (testIt == registeredBotTypes.end())
		return false;
	else
		return true;
}

Bot* BotRecycler::retrieveBot(wstring botType)
{
	// FIRST MAKE SURE THIS IS A REGISTERED BOT TYPE,
	// IF IT IS NOT, WE NEED TO RETURN NULL
	if (isRegisteredBotType(botType))
	{
		return NULL;
	}
	else
	{
		// GET THE CORRECT LIST OF BOT TYPES
		list<Bot *> *botsOfTypeWeNeed = recyclableBots[botType];

		// MAKE SURE WE ARE NOT OUT OF THIS TYPE OF BOT
		if(botsOfTypeWeNeed->size() == 0)
			addMoreBots(botsOfTypeWeNeed, botType, RECYCLABLE_BOT_INCREMENT);

		// NOW GET THE LAST ELEMENT
		Bot* botToReturn = botsOfTypeWeNeed->back();
		botsOfTypeWeNeed->pop_back();
		return botToReturn;
	}
}

void BotRecycler::recycleBot(wstring botType, Bot* botToRecycle)
{
	if (isRegisteredBotType(botType))
	{
		list<Bot *> *recycleBots = recyclableBots[botType];
		recycleBots->push_back(botToRecycle);
	}
}

void BotRecycler::addMoreBots(list<Bot *> *botsNeededList, wstring botType, unsigned int numToAdd)
{
	// NOW MAKE THE BOTS
	Bot* sampleBot = registeredBotTypes[botType];

	for (unsigned int i = 0; i < numToAdd; i++)
	{
		Bot *botClone = sampleBot->clone();
		botsNeededList->push_back(botClone);
	}
}
