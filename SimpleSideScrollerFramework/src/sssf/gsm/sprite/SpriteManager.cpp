/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\state\GameStateManager.h"

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();
	float rotation = sprite->getRotationInRadians();

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									pp->getX(),
									pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(sprite->getCurrentImageID(),
			pp->round(pp->getX() - viewport->getViewportX()),
			pp->round(pp->getY() - viewport->getViewportY()),
			pp->round(pp->getZ()),
			sprite->getAlpha(),
			spriteType->getTextureWidth(),
			spriteType->getTextureHeight(),
			rotation);
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(&player, renderList, viewport);

		// ADD THE BOTS
		list<Bot*>::iterator botsIterator;
		botsIterator = bots.begin();
		while (botsIterator != bots.end())
		{
			Bot *bot = *botsIterator;
			addSpriteToRenderList(bot, renderList, viewport);
			botsIterator++;
		}

		// NOW ADD THE REST OF THE SPRITES
		list<LifelessObject*>::iterator objectIterator;
		objectIterator = objects.begin();
		while (objectIterator != objects.end())
		{			
			LifelessObject *object = (*objectIterator);
			addSpriteToRenderList(object, renderList, viewport);
			objectIterator++;
		}
	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addLifelessObject(LifelessObject *objectToAdd)
{
	objects.push_back(objectToAdd);
}

void SpriteManager::addBot(Bot* botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	objects.clear();
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
	bots.clear();
	objects.clear();
}

void SpriteManager::removeLifelessObject(LifelessObject *objectToRemove)
{
	objects.remove(objectToRemove);
}

void SpriteManager::removeBot(Bot* botToRemove)
{
	bots.remove(botToRemove);
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// UPDATE THE PLAYER SPRITE
	player.updateSprite();

	// UPDATE VIEWPORT
	updateViewport(game);

	// UPDATE BOTS
	list<Bot*>::iterator botsIterator;
	botsIterator = bots.begin();
	while (botsIterator != bots.end())
	{
		Bot *bot = *botsIterator;
		//bot->think(game);
		bot->updateSprite();
		botsIterator++;
	}

	// NOW UPDATE LIFELESS OBJECTS
	list<LifelessObject*>::iterator objectIterator;
	objectIterator = objects.begin();
	while (objectIterator != objects.end())
	{
		LifelessObject *object = (*objectIterator);
		object->updateSprite();
		objectIterator++;
	}
}

void SpriteManager::updateViewport(Game* game)
{
	Viewport* viewport = game->getGUI()->getViewport();

	PhysicalProperties* pp = player.getPhysicalProperties();

	float playerCenterX = pp->getX() + 64;
	float playerCenterY = pp->getY() + 128;
	float viewportCenterX = viewport->getViewportCenterX();
	float viewportCenterY = viewport->getViewportCenterY();
	float viewportX = viewport->getViewportX();
	float viewportY = viewport->getViewportY();

	float velX = 0.0f;
	float velY = 0.0f;

	if (playerCenterX < viewportCenterX - 192) velX = -15.0f;
	else if (playerCenterX > viewportCenterX + 192) velX = 15.0f;
	if (playerCenterY < viewportCenterY - 192) velY = -15.0f;
	else if (playerCenterY > viewportCenterY + 192) velY = 15.0f;

	viewport->moveViewport(velX, velY,
		game->getGSM()->getWorld()->getWorldWidth(),
		game->getGSM()->getWorld()->getWorldHeight());
}