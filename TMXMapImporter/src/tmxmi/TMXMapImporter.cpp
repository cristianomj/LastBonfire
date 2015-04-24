#include "tmxmi_VS\stdafx.h"
#include "tmxmi\TMXMapImporter.h"
#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h";
#include "sssf\game\Game.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "xmlfi\XMLFileImporter.h"

bool TMXMapImporter::loadWorld(Game *game, wstring initDir, wstring mapLevelFileName)
{
	dir = initDir;
	mapLevelFilePath = dir + mapLevelFileName;
	bool success = loadMapInfo();
	if (!success) return false;
	success = buildWorldFromInfo(game);
	if (!success) return false;
	return true;
}

bool TMXMapImporter::loadMapInfo()
{
	// LOAD THE XML DOC
	const char *charPath = newCharArrayFromWstring(mapLevelFilePath);
	TiXmlDocument doc(charPath);
	delete charPath;
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *pElem = doc.FirstChildElement();
		string eName;
		if (pElem)
		{
			eName = pElem->Value();
			if (strcmp(eName.c_str(), MAP_ELEMENT.c_str()) == 0)
			{
				// WHAT'S THE TYPE OF MAP?
				const char* orientation = pElem->Attribute(ORIENTATION_ATT.c_str());
				if (strcmp(orientation, ORTHOGONAL_VAL.c_str()) == 0)
				{
					mapType = MapType::ORTHOGONAL_MAP;
					return loadOrthographicMap(pElem);
				}
				else
				{
					mapType = MapType::ISOMETRIC_MAP;
					return loadIsometricMap(pElem);
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}



bool TMXMapImporter::loadOrthographicMap(const TiXmlElement *pElem)
{
	this->width = extractIntAtt(pElem, WIDTH_ATT);
	this->height = extractIntAtt(pElem, HEIGHT_ATT);
	this->tileWidth = extractIntAtt(pElem, TILEWIDTH_ATT);
	this->tileHeight = extractIntAtt(pElem, TILEHEIGHT_ATT);

	const TiXmlNode *node = pElem->FirstChild();
	while (node)
	{
		string eName = node->Value();
		if (strcmp(eName.c_str(), TILESET_ELEMENT.c_str()) == 0)
		{
			loadTileSetInfo(node);
		}
		else if (strcmp(eName.c_str(), IMAGELAYER_ELEMENT.c_str()) == 0)
		{
			loadImageLayerInfo(node);
		}
		else if (strcmp(eName.c_str(), LAYER_ELEMENT.c_str()) == 0)
		{
			loadTiledLayerInfo(node);
		}
		node = node->NextSibling();
	}
	return true;
}

void TMXMapImporter::loadImageLayerInfo(const TiXmlNode *node)
{
	const TiXmlElement *element = node->ToElement();
	ImageLayerInfo imageLayerInfo;
	imageLayerInfo.name = extractCharAtt(element, NAME_ATT);
	// WE DON'T ACTUALLY CARE ABOUT THE TMX WIDTH AND HEIGHT FOR THIS

	// NOW GET THE IMAGE ELEMENT
	const TiXmlNode *childNode = node->FirstChild();
	imageLayerInfo.imageSource = extractCharAtt(childNode->ToElement(), SOURCE_ATT);

	// AND NOW GET IMAGE LAYER PROPERTIES
	childNode = childNode->ToElement()->NextSibling();
	if (node != NULL)
	{
		string eName = childNode->Value();
		if (strcmp(eName.c_str(), PROPERTIES_ELEMENT.c_str()) == 0)
		{
			const TiXmlNode *grandchildNode = childNode->FirstChild();
			while (grandchildNode != NULL)
			{
				const TiXmlElement *grandchildElement = grandchildNode->ToElement();
				string att = grandchildElement->Attribute(NAME_ATT.c_str());
				if (strcmp(att.c_str(), COLLIDABLE_ATT.c_str()) == 0)
				{
 					imageLayerInfo.collidable = extractBoolAtt(grandchildNode->ToElement(), VALUE_ATT);
				}
				else if (strcmp(att.c_str(), IMAGEHEIGHT_ATT.c_str()) == 0)
				{
					imageLayerInfo.imageheight= extractIntAtt(grandchildNode->ToElement(), VALUE_ATT);
				}
				else if (strcmp(att.c_str(), IMAGEWIDTH_ATT.c_str()) == 0)
				{
					imageLayerInfo.imagewidth = extractIntAtt(grandchildNode->ToElement(), VALUE_ATT);
				}
				grandchildNode = grandchildNode->NextSibling();
			}
		}
	}
	// OK, NOW LOAD THE LAYER
	imageLayerInfos[imageLayerInfo.name] = imageLayerInfo;
}



void TMXMapImporter::loadTiledLayerInfo(const TiXmlNode *node)
{
	const TiXmlElement *element = node->ToElement();
	TiledLayerInfo tiledLayerInfo;
	tiledLayerInfo.name = extractCharAtt(element, NAME_ATT);
	tiledLayerInfo.width = extractIntAtt(element, WIDTH_ATT);
	tiledLayerInfo.height = extractIntAtt(element, HEIGHT_ATT);

	// NOW GET THE PROPERTIES
	const TiXmlNode *propertiesNode = node->FirstChild();
	string eName = propertiesNode->Value();
	const TiXmlNode *dataNode;
	if (strcmp(eName.c_str(), PROPERTIES_ELEMENT.c_str()) == 0)
	{
		const TiXmlNode *propNode = propertiesNode->FirstChild();
		while (propNode != NULL)
		{
			const TiXmlElement *propElement = propNode->ToElement();
			string att = propElement->Attribute(NAME_ATT.c_str());
			if (strcmp(att.c_str(), COLLIDABLE_ATT.c_str()) == 0)
			{
				tiledLayerInfo.collidable = extractBoolAtt(propNode->ToElement(), VALUE_ATT);
			}
			propNode = propNode->NextSibling();
		}
		dataNode = propertiesNode->NextSibling();
	}

	// NOW GET THE TILE DATA
	if (dataNode == NULL)
	{
		dataNode = node->FirstChild();
	}
	const TiXmlNode *tileNode = dataNode->FirstChild();
	while (tileNode)
	{
		const TiXmlElement *element = tileNode->ToElement();
		int gid = extractIntAtt(element, GID_ATT);
		tiledLayerInfo.gids.push_back(gid);
		tileNode = tileNode->NextSibling();
	}
	tiledLayerInfo.tileSetInfo = getTileSetForId(tiledLayerInfo.gids[0]);
	tiledLayerInfos[tiledLayerInfo.name] = tiledLayerInfo;
}

void TMXMapImporter::loadTileSetInfo(const TiXmlNode *node)
{
	const TiXmlElement *element = node->ToElement();

	TileSetInfo tileSetInfo;
	tileSetInfo.firstgid = extractIntAtt(element, FIRSTGID_ATT);
	tileSetInfo.name = extractCharAtt(element, NAME_ATT);
	tileSetInfo.tilewidth = extractIntAtt(element, TILEWIDTH_ATT);
	tileSetInfo.tileheight = extractIntAtt(element, TILEHEIGHT_ATT);

	// NOW GET THE IMAGE INFO
	node = node->FirstChild();
	element = node->ToElement();
	tileSetInfo.sourceImage = extractCharAtt(element, SOURCE_ATT);
	tileSetInfo.sourceImageWidth = extractIntAtt(element, WIDTH_ATT);
	tileSetInfo.sourceImageHeight = extractIntAtt(element, HEIGHT_ATT);

	tileSetInfos[tileSetInfo.name] = tileSetInfo;
}

bool TMXMapImporter::loadIsometricMap(const TiXmlElement *pElem)
{
	return true;
}

bool TMXMapImporter::buildWorldFromInfo(Game *game)
{
	TextureManager *worldTextureManager = game->getGraphics()->getWorldTextureManager();
	if (mapType == MapType::ORTHOGONAL_MAP)
	{
		World *world = game->getGSM()->getWorld();
		int largestLayerWidth = 0;
		int largestLayerHeight = 0;

		// LET'S FIRST FIGURE OUT THE WORLD WIDTH AND HEIGHT

		// FIRST THE IMAGE LAYERS
		map<string, ImageLayerInfo>::const_iterator iliIt = imageLayerInfos.begin();
		while (iliIt != imageLayerInfos.end())
		{
			string key = iliIt->first;
			ImageLayerInfo ili = imageLayerInfos[key];
			if (ili.imagewidth > largestLayerWidth)
				largestLayerWidth = ili.imagewidth;
			if (ili.imageheight > largestLayerHeight)
				largestLayerHeight = ili.imageheight;

			iliIt++;
		}
		// AND THE TILED LAYERS
		map<string, TiledLayerInfo>::const_iterator tliIt = tiledLayerInfos.begin();
		while (tliIt != tiledLayerInfos.end())
		{
			string key = tliIt->first;
			TiledLayerInfo tli = tiledLayerInfos[key];
			int layerWidth = tli.width * tli.tileSetInfo->tilewidth;
			if (layerWidth > largestLayerWidth)
				largestLayerWidth = layerWidth;
			int layerHeight = tli.height * tli.tileSetInfo->tileheight;
			if (layerHeight > largestLayerHeight)
				largestLayerHeight = layerHeight;
			tliIt++;
		}
		unsigned int idOffset = worldTextureManager->getWStringTable()->getNumWStringsInTable();

		// FIRST LOAD ALL THE TILE SETS
		map<string, TileSetInfo>::const_iterator tsiIt = tileSetInfos.begin();
		while (tsiIt != tileSetInfos.end())
		{
			string key = tsiIt->first;
			TileSetInfo tsi = tileSetInfos[key];
			wstring sourceImageW(tsi.sourceImage.begin(), tsi.sourceImage.end());
			bool success = worldTextureManager->loadTileSetFromTexture(game, dir, sourceImageW, tsi.tilewidth, tsi.tileheight);
			if (!success) return false;
			tsiIt++;
		}

		// NOW LOAD THE IMAGE LAYERS, IF THERE ARE ANY
		iliIt = imageLayerInfos.begin();
		while (iliIt != imageLayerInfos.end())
		{
			string key = iliIt->first;
			ImageLayerInfo ili = imageLayerInfos[key];
			
			TiledLayer *imageLayerToAdd = new TiledLayer(	1,
				1,
				ili.imagewidth,
				ili.imageheight,
				0,
				ili.collidable,
				largestLayerWidth,
				largestLayerHeight);
			world->addLayer(imageLayerToAdd);

			Tile *imageTile = new Tile();
			imageTile->collidable = ili.collidable;
			wstring imageSourceW(ili.imageSource.begin(), ili.imageSource.end());
			imageTile->textureID = worldTextureManager->loadTexture(dir + imageSourceW);
			imageLayerToAdd->addTile(imageTile);
	
			iliIt++;
		}

		// AND NOW LOAD THE TILED LAYERS, WHICH REFERENCE THE TILE SETS
		tliIt = tiledLayerInfos.begin();
		while (tliIt != tiledLayerInfos.end())
		{
			// @TODO WE'LL NEED TO CUSTOMIZE THIS
			bool collidableLayer = false;
			string key = tliIt->first;
			TiledLayerInfo tli = tiledLayerInfos[key];
			TiledLayer *tiledLayerToAdd = new TiledLayer(	tli.width,
															tli.height,
															tli.tileSetInfo->tilewidth,
															tli.tileSetInfo->tileheight,
															0,
															tli.collidable,
															largestLayerWidth,
															largestLayerHeight);
			world->addLayer(tiledLayerToAdd);

			// WE HAVE TO ADD ALL THE TILES
			int row = 0;
			int col = 0;
			int uncollidableIndex = tli.tileSetInfo->firstgid;
			for (unsigned int i = 0; i < tli.gids.size(); i++)
			{
				Tile *tileToAdd = new Tile();
				tileToAdd->textureID = tli.gids[i] + idOffset - 1;
				if (tli.gids[i] == uncollidableIndex)
					tileToAdd->collidable = false;
				else
					tileToAdd->collidable = tli.collidable;
				tiledLayerToAdd->addTile(tileToAdd);
			}
			tliIt++;
		}

		// AND MAKE THE WORLD DIMENSIONS THE
		// SIZE OF THE LARGEST LAYER
		world->setWorldWidth(largestLayerWidth);
		world->setWorldHeight(largestLayerHeight);
	}
	return true;
}

TileSetInfo* TMXMapImporter::getTileSetForId(int id)
{
	map<string, TileSetInfo>::const_iterator it = tileSetInfos.begin();
	while (it != tileSetInfos.end())
	{
		string key = it->first;
		TileSetInfo tsi = tileSetInfos[key];
		int columns = tsi.sourceImageWidth/tsi.tilewidth;
			int rows = tsi.sourceImageHeight/tsi.tileheight;
		int numTiles = rows * columns;
		if ((id >= tsi.firstgid) && (id < (tsi.firstgid + numTiles)))
		{
			return &tileSetInfos[key];
		}
		it++;
	}
	return NULL;
}